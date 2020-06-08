# ZINLIDAC

zinlidac是我用C++和Python写的一个在Linux上的Live Data Collection工具。zinlidac的名字是ZINLIDAC Is Not a LIve DAta Collector的缩略词。

## 框架结构

![framework](assets/framework.pdf)

本框架的结构分为三个部分：

* 由C++编写的`libzinlidac.so`库，负责调用系统API收集系统资源（`lib`子目录）
* 由C++编写的`libzinlidacpy.so`库，负责将`libzinlidac.so`库封装成Python接口（`python-wrapper`子目录）
* 由Python编写的用户输入处理与输出序列化部分（`python`子目录）

### 设计理由

这样设计结构的原因为：

* 由于系统底层的资源获取大多都是提供原生的C接口或系统调用，因此，实际收集系统资源的部分应该由C或C++编写。
* 但是，也正因为C或C++较为底层，因此其较缺乏进行复杂数据处理、加工的库，所以使用高级语言对其进行封装。
* 这里选择使用Python而不是Java的原因在于，数据收集程序一般是在U盘中对别的机器进行访问，而Java需要附带一整个JDK，显得较为笨重。

本框架的用户输入为GraphQL格式，输出为JSON格式，这样设计的原因在于：

* 用户计算机内会有各种各样的Live Data，如CPU、内存、网络、进程等，但有时并不是所有的数据都是需要收集的。因此，最灵活的方法就是让用户指定收集哪些数据。但是，由于其种类繁多，让用户在命令行参数内输入数组的形式显然很麻烦。因此，针对这种多字段的复杂选择，我采用了行业内最近比较流行的方法GraphQL（[官方网站](https://graphql.github.io)），其特点为可以提供类型安全的，多字段的，嵌套的任意查询。因此，使用这种格式作为输入，大大提高了用户的个性化Live Data收集的能力，以及增强了框架的可拓展能力。
* 收集到的Live Data数据需要进一步的分析，因此，需要将数据进行妥善的存储。我这里采用的是使用JSON来进行序列化，因为其支持字符串、数字等多种数据格式的序列化，并且由于其使用广泛，多种语言、工具、框架都有解析JSON文件的能力，所以能最大程度减少后续数据处理的反序列化与格式转换。<br />但是，也有可以提升的地方。由于数据采集下来并不是立即查看，所以其文件的可读性不需要太强，而且由于数据需要经过U盘的移动，所以应尽可能地压缩数据。所以，可以考虑采用二进制序列化的行业标准Protocol Buffers（[官方网站](https://developers.google.com/protocol-buffers/)）. 但是，经过实际考察和试验，其需要编写复杂的`.proto`文件，并且由于每个字段是采用数字作为标记，所以不能序列化动态字典，这样就失去了使用GraphQL时带来的巨大灵活性，所以最终放弃了这一方案。

### 优势

本框架的优势在于：

* 将核心功能封装成使用C++编写的`libzinlidac.so`的动态链接库，能够最大程度与其他语言进行交互，可以极其方便地移植到别的接口中，而不需要更改太多的代码。
* `libzinlidac.so`库内进行模块化编写，可扩展性强，同时也便于代码阅读与维护。
* `libzinlidac.so`库直接使用系统提供的API及系统调用进行数据收集，降低了过程开销，并且严格按照相应的API指南进行编程，收集的数据种类较多。
* `libzinlidacpy.so`库使用的boost.python框架是目前C++与Python交互最好的框架，既降低了开发者的操作难度，也提供了丰富的API，将C++的数据结构封装为完整的Python结构。
* C++模块采用bazel作为项目构建工具，现代化且可定制性强。
* Python主函数部分，使用GraphQL官方提供的库Graphene（[官方网站](https://graphene-python.org)）作为输入处理框架，使用原生的JSON框架作为输出序列化框架，既符合行业标准，也提供了巨大的灵活性，并且也与其他程序可以进行较强的互动。

## Live Data收集原理

本实验收集的Live Data可以汇总为以下GraphQL schema：

```graphql
type LibzinlidacResult{
	hardware: Hardware
	system: System
	command(commands: [String]): [Command]
	file(srcs: [String], base_dir: String): [File]
}
```

分为四大类：

* `hardware`<br />硬件类数据，由C++收集
* `system`<br />系统类数据，由C++收集
* `command`<br />执行指令获得相应的数据，由Python收集
* `file`<br />获得部分系统文件，由Python收集

这样分类的原因在于，`hardware`和`system`类大多都是直接调用系统API或系统调用来获得数据，但是有些数据并不能用用户态程序获得，只能通过执行系统提供的命令或者观察对应文件获得，而这类操作使用Python最合适。

### 硬件类

硬件类数据的程序代码位于`lib/hardware`子目录下，可以汇总为以下GraphQL schema：

```graphql
type Hardware {
	cpu: CPU
	cache: Cache
	memory: Memory
}
type CPU {
	cpus_number: Int
	cpu_clock_ticks_per_time: Int
	word_bit: Int
	boot_time: Int
	cpuinfo: String
	cpu_stat: String
	hardware_type: String
}
type Cache {
	level1_instruction_cache_size: Int
	level1_data_cache_size: Int
	level2_cache_size: Int
	level3_cache_size: Int
	level4_cache_size: Int
}
type Memory {
	virtual_memory_page_size: Int
	physical_memory_page_size: Int
	available_physical_memory_pages_number: Int
	shared_memory_size: Int
	swap_space_size: Int
	meminfo: String
}
```

这类数据的主要收集方式是使用`<unistd.h>`系统库中提供的`sysconf`函数，其签名如下：

```C
long sysconf(int name);
```

其中`name`的值可以使用`man sysconf`查看其使用指南来确定。

每个字段的名称都较清楚地表达了其作用，关于每条字段的详细作用可以查看`lib/hardware/hardware.h`文件，每个函数都有详尽的注释。

### 系统类

系统类数据的程序代码位于`lib/system`子目录下，可以汇总为以下GraphQL schema：

```graphql
type System {
	operating_system: OperatingSystem
	filesystem: Filesystem
	time: Time
	locale: Locale
	user: User
	process: Process
	network: Network
}
```

#### Operating system

这类数据可以汇总为如下GraphQL schema:

```graphql
type OperatingSystem {
	system_name: String
	system_release_level: String
	system_release_version_level: String
}
```

这类数据是与操作系统版本有关的数据，其获得方法是调用`<sys/utsname.h>`系统库的`uname`函数，其签名如下：

```C
int uname(struct utsname *buf);
```

其中的`struct utsname`就含有相应的数据。

#### Filesystem

这类数据可以汇总为如下GraphQL schema:

```graphql
type Filesystem {
	filesystem_capacity: Int
	filesystem_free_size: Int
	filesystem_type: String
	current_mounted_filesystems: [FilesystemInfo]
	configured_mounted_filesystems: [FilesystemInfo]
}
type FilesystemInfo {
	name: String
	dir: String
	mount_type: String
	mount_options: String
}
```

这类数据与文件系统有关，其中，关于文件系统容量部分的三个字段是由`<sys/statvfs.h>`系统库中的`statvfs`函数提供，其签名如下：

```C
int statvfs(const char *path, struct statvfs *buf);
```

其中`struct statvfs`结构体就提供了有关容量的初始数据字段，包括文件系统片段大小`f_frsize`，文件系统的总片段数`f_blocks`, 文件系统的空闲片段数`f_bfree`, 我们只需要根据其相应的关系得到相应的需求字段：

```C++
filesystem_capacity = buf->f_frsize * buf->f_blocks;
filesystem_free_size = buf->f_bfree * buf->f_blocks;
```

其实在C++17中提供了相应的标准库`<filesystem>`, 但是由于大多数系统中的C++版本还只是C++11，所以这里依然采用原始的系统API方式。

而剩余的两个关于多文件系统的字段，则是使用`<mntent.h>`系统库的`getmntent`函数，其签名如下：

```c
struct mntent *getmntent(FILE *stream);
```

其原理为，系统用于记录多文件系统的信息文件为`/proc/fstab`和`/proc/mtab`, 其分别记录了系统配置启动时挂载的文件系统，和当前挂载的文件系统。该函数通过读取相应的文件，生成`struct mntent`结构体，其内容就是相应的需求字段。

#### Time

这类数据可以汇总为如下GraphQL schema:

```graphql
type Time {
	current_time: Int
	timezone: String
}
```

这类数据与时间有关。获取当前时间是使用的`<sys/time.h>`系统库的`gettimeofday`函数，其签名如下：

```c
int gettimeofday(struct timeval *tv, struct timezone *tz);
```

其中`struct timeval`返回的就是当前的时间，实际上是以秒计的自1970年1月1日的时间，也就是常用的Unix时间戳。

而获取当前时区的方法是直接读取`/etc/timezone`文件。

#### Locale

这类数据可以汇总为如下GraphQL schema:

```graphql
type Locale {
	locale: String
	codeset: String
}
```

这类数据与locale（我不知道怎么翻译）有关。获取当前的locale是直接使用的C++标准库中的`<locale`>库中的`std::locale`类，可以直接访问其`name`字段获得相应的值。

获取当前的编码方式是使用`<langinfo.h>`系统库的`nl_langinfo`函数，其签名如下：

```c
char *nl_langinfo(nl_item item);
```

其通过读取不同的`nl_item`返回不同的值，而`nl_item`的所有可能的值都记录在`<langinfo.h>`中。这里使用的就是`CODESET`.

#### User

这类数据可以汇总为如下GraphQL schema:

```graphql
type User {
	associated_user: LoginUserInfo
	users: [UserInfo]
	groups: [GroupInfo]
}
type LoginUserInfo {
	name: String
	uid: Int
}
type UserInfo {
	name: String
	uid: Int
	groupid: Int
	full_name: String
	home_directory: String
	login_shell: String
}
type GroupInfo {
	name: String
	gid: Int
	users: [String]
}
```

这类数据与系统用户有关。获得当前登陆用户的信息的方法是使用`<unistd.h>`系统库提供的`getlogin`和`getuid`函数，其签名如下：

```c
char *getlogin(void);
uid_t getuid(void);
```

`getlogin`返回当前登陆用户的用户名，`getuid`返回该进程对应用户的uid。

系统并没有提供获取所有用户信息和所有用户组信息的API，但是，其将相应的信息分别记录在了`/etc/passwd`和`/etc/group`文件中。其格式可以通过`man --section=5 passwd`和`man --section=5 group`查看如下：

* `/etc/passwd`（以`:`分隔）
    * 登录名
    * 加密的密码（一般用`*`表示）
    * 用户的uid
    * 用户的gid
    * 用户全名
    * 用户的`home`目录
* `/etc/group`（以`:`分隔）
    * 组名
    * 密码（可以为空）
    * gid
    * 用户列表（以`,`分隔）

根据上述的格式，读取相应的文件，使用正则表达式匹配后就可获得相应的数据。

#### Process

这类数据可以汇总为如下GraphQL schema:

```graphql
type Process {
	environment_variables: [String]
	processes: [ProcessInfo]
	path_environment: String
}
type ProcessInfo {
	ppid: Int
	utime: Int
	stime: Int
	start_time: Int
	priority: Int
	nice: Int
	vm_size: Int
	vm_resident: Int
	vm_share: Int
	environment_variables: [String]
	cmdline_arguments: [String]
	user: String
	basename: String
	threads_number: Int
}
```

这类数据与进程相关。读取该进程的环境变量，可以直接声明全局的`extern char **environ`。而获得`PATH`的环境变量，可以使用`<stdlib.h>`系统库提供的`getenv`函数，其签名如下：

```c
char *getenv(const char *name);
```

通过`name`获得对应的环境变量值。

而获得所有进程的信息，需要安装`libprocps-dev`库：

```shell
apt install libprocps-dev
```

并且在链接时加上`-lprocps`选项。

然后使用`<proc/readproc.h>`库的函数`openproc`, `readproc`来完成，其签名如下：

```c
PROCTAB *openproc(int flags, ...);
proc_t *readproc(PROCTAB *PT, proc_t *return_buf);
```

其原理为，由于Linux系统将各个进程的信息分目录存储在`/proc`目录下，所以这个库通过这两个函数打开并读取位于`/proc`目录下的各进程信息，进行分析、处理后返回结果，而本工具提供的相应信息就位于`proc_t`结构体中。

#### Network

这类数据可以汇总为如下GraphQL schema:

```graphql
type Network {
	network_interfaces: [NetworkInterface]
	lsof_i: [LsofIResult]
	lsof_Ua: [LsofUaResult]
}
type NetworkInterface {
	name: String
	network_family: String
	address: String
}
type LsofIResult {
	command: String
	pid: Int
	user: String
	fd: String
	type: String
	device: String
	node: String
	name: String
}
type LsofUaResult {
	command: String
	pid: Int
	user: String
	fd: String
	type: String
	device: String
	name: String
}
```

这类数据与网络信息有关。最重要的字段就是获取所有网络接口数据，我是通过使用`<ifaddrs.h>`系统库的`getifaddrs`函数实现的，其签名如下：

```c
int getifaddrs(struct ifaddrs **ifap);
```

其中`struct ifaddrs`是一个链表，存储有每个网络接口的信息，包括接口名`ifa_name`, ``struct sockaddr`类型的接口地址`ifa_addr`。因此，获取接口名就十分方便，直接访问`ifa_name`字段即可。而该接口的协议族就需要访问`ifa_addr`的`sa_family`字段，但是其是一些预先定义为整型的宏，所以我就设置了一张值与名称对应的表来获得协议族名称。而获取网络接口地址则相对较为复杂，需要根据协议族的不同来采用不同的方法。若协议族为`AF_INET`或`AF_INET6`，即IPv4或IPv6，那么只需要调用`<sys/socket.h>`系统库的`getnameinfo`函数，其签名如下：

```c
int getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags);
```

通过传入`struct sockaddr`来获得相应的地址。

而如果协议族为`AF_PACKET`, 也就是数据链路层的接口，那么根据stackoverflow网站上的一篇回答[MAC address with getifaddrs](https://stackoverflow.com/a/6762962/10005095)，只需要将`struct sockaddr`强制转化为`struct sockaddr_ll`即可：

```c
struct sockaddr_ll *s = (struct sockaddr_ll*)(interface_address->ifa_addr);
```

然后访问其`sll_addr`字段即可。

接着是关于进程和对应网络端口的数据获取。这些数据系统依然没有提供相应的API可以直接获取，但是将其数据存储在了`/proc/*/net`相应的目录下，其内容多而繁杂，并且难以直接处理。因此，这里被迫调用系统提供的命令行工具，然后通过`popen`来获取其输出值，然后再使用正则表达式来处理。这里使用的命令行工具为`lsof`，其中`lsof -Ua`列出进程打开的所有文件描述符，而`lsof -i`列出进程打开的所有与网络相关的文件描述符。这个处理方式十分不优雅，因此不再赘述。

### 指令类

这一部分的代码位于`python/schema/command.py`，指令类的数据的GraphQL schema如下：

```graphql
type Command {
	command: String
	result: String
}
```

其中，`command`字段表示该指令自身的字符串，`result`字段表示执行该指令后得到的结果。

引入指令类的原因在于，Linux系统的系统数据并不是全部提供用户态API的获取接口的，有的是内核态的API接口，有的是数据存储在文件中而不提供接口的。因此，仅用C++调用系统提供的接口来获取的数据并不全面，并且系统中也会提供成熟的工具。因此，指令类的作用就在于让用户可以直接调用系统中的工具来进行数据收集。

用Python实现这一功能十分方便，只需要使用`subprocess`子模块，调用`subprocess.run()`函数，并将标准输出、标准错误重定向至`subprocess.PIPE`，再进行错误处理即可。其实现代码为

```python
def execute_command(command):
    try:
        proc = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True, shell=True)
        return proc.stdout.decode('utf-8')
    except subprocess.CalledProcessError as e:
        raise Exception(e.stderr.decode('utf-8'))
```

下面列举一些常见的指令：

* `uname -a`<br />获取系统信息，如内核版本号等
* `fdisk -l`<br />展示所有硬盘分区信息
* `df -h`<br />展示硬盘容量信息
* `dpkg --get-selections`<br />展示所有使用包管理器安装的软件包
* `crontab -l`<br />展示所有使用`cron`进行定时任务的进程
* `service --status-all`<br />展示所有当前的服务，其中`[ + ]`代表正在运行的服务，`[ - ]`代表已经停止的服务，`[ ? ]`代表无法使用`status`命令的服务
* `lsmod`<br />展示Linux内核中所有模块的状态
* `ifconfig -a`<br />展示当前所有可用的网络接口的信息
* `netstat -rn`<br />展示内核的路由表
* `netstat -anp`<br />展示系统当前的所有套接字
* `lsof -i`<br />展示所有打开与网络有关的文件描述符的进程
* `lsof -Ua`<br />展示所有进程与其打开的文件描述符
* `arp -a`<br />展示当前系统的ARP缓存

此外，我们还可以在这里使用自带的软件，比如说LiME（[504ensicsLabs/LiME](https://github.com/504ensicsLabs/LiME)）等获取内存快照的软件。这里以LiME为例：

```shell
insmod path/to/lime.ko "path=<outfile> format=lime"
```

可以获取当前的内存快照并以lime格式保存至`outfile`中。值得注意的是，得根据内核版本选择不同的LiME版本进行获取，所以运行此命令时必须得有符合当前系统内核版本的LiME。

### 文件类

这一部分的代码位于`python/schema/file.py`，指令类的数据的GraphQL schema如下：

```graphql
type File {
	src: String
	dst: String
}
```

其中，`src`字段为该文件在系统中的原路径，`dst`为将该文件拷贝的目标路径。该指令将系统中的重要文件拷贝至指定位置。

除了使用系统API，系统调用，系统提供的软件工具可以获得系统数据之外，Linux系统还有一个显著特征，有的重要数据是保存在特定位置的文件中，且只能通过读取相关的文件来获得相应信息，不提供系统API。因此，我们可以使用文件类来进行这类数据的获取，将重要文件拷贝至取证的U盘里。

Linux比较重要的数据文件主要在`/proc`目录，`/etc`目录和用户的`home`目录下。`/proc`目录上挂载的是Linux系统制造的一个伪文件系统，提供了获得内核数据结构的接口。其主要的文件的意义可以通过`man --section=5 proc`命令进行查看。`/etc`目录里主要是系统的配置文件，用户自己的文件、数据主要存储在用户的`home`目录里。

常见的重要文件包括：

* `/proc/cpuinfo`<br />包含当前计算机CPU每个处理器的详细信息
* `/proc/diskstats`<br />包含当前计算机硬盘I/O的数据
* `/proc/meminfo`<br />包含当前计算机主存储器的详细信息
* `/proc/modules`<br />包含当前系统加载的所有模块
* `/proc/net/arp`<br />包含当前内核的ARP表
* `/proc/net/tcp`<br />包含当前内核的TCP套接字表
* `/proc/net/udp`<br />包含当前内核的UDP套接字表
* `/proc/partitions`<br />包含当前计算机硬盘的所有分区
* `/proc/stat`<br />内核/系统数据
* `/proc/vmstat`<br />包含虚存数据
* `/etc/issue`<br />包含了在登录提示前出现的信息或系统标识符。
* `/etc/passwd`<br />包含了所有用户的信息
* `/etc/group`<br />包含了所有用户组的信息
* `~/.xxsh_history`<br />包含了当前用户shell的命令历史。需要根据该用户的shell来确定文件名，如用bash的用户相应的文件名为`~/.bash_history`, 用zsh的用户相应的文件名为`~/.zsh_history`. 用户登录时的shell可以在`/etc/passwd`中查看
* `~/.xxsh_rc`<br />包含了当前用户shell的配置，与`~/.xxsh_history`类似

## 编译安装

本工具采用C++和Python编写，在Ubuntu 18.04上进行了测试，所需要安装的工具有：

* bazel（[官方网站](https://bazel.build)）<br />现代化C++项目构建工具。版本在3.2.0及以上。
* g++或clang++<br />C++编译器。版本要求支持C++11。
* python<br />Python解释器。版本要求在3.6及以上。

在编译之前，需要安装的包有：

* libprocps<br />在C++代码中用于文件系统相关数据的读取。可通过`apt install libprocps-dev`下载安装。
* numpy<br />仅作为boost.python的依赖出现，并无实际作用。

在安装python和numpy结束后，需要将项目根目录下的`WORKSPACE`文件的第18行和第37行的`path`字段改为自己计算机中相应的路径。

### `libzinlidac.so`

如果需要将本工具作为动态链接库使用，而不直接使用Python提供的接口，可以只编译其核心模块。其编译指令为

```shell
bazel build //lib:libzinlidac.so
```

如果需要指定使用clang++作为编译器，只需要添加`CC`的环境变量：

```shell
CC=clang bazel build //lib:libzinlidac.so
```

### `libzinlidacpy.so`

如果不需要将工具作为动态链接库使用，而是使用本工具提供的Python接口，那么需要编译python的封装模块。其编译指令为

```shell
bazel build //python-wrapper:libzinlidacpy.so
```

如果需要指定使用clang++作为编译器，只需要添加`CC`的环境变量：

```shell
CC=clang bazel build //python-wrapper:libzinlidacpy.so
```

### Python模块

首先，需要安装Python模块需要的包，其需求文件位于`python/requirements.txt`文件。

然后，需要将上一步编译好的`libzinlidacpy.so`文件放置在`python`目录下。如果不对bazel进行额外配置，其默认是位于`bazel-bin/python-wrapper/libzinlidacpy.so`路径下。

然后就可以通过以下指令使用Python接口了：

```shell
python path/to/main.py path/to/input.graphql --output path/to/results.json
```

其中，`input.graphql`为用于指明需要收集哪些数据的GraphQL文件，`results.json`为输出的JSON结果文件。

## 使用说明

Python接口通过`input.graphql`来对所需要收集的工具进行灵活的定制。本项目在`python/templates/standard.graphql`提供了一个标准的可运行的GraphQL文件，用于一般的比较简略的Live Data的收集。当然，用户也可以自己定制GraphQL文件：

用户可以根据本文在收集原理中提到的GraphQL schema来写特定的GraphQL查询语句。由于GraphQL的灵活性，可以只指定自己需要的字段进行查询，不需要的字段可以直接省略，这点在`hardware`类和`system`类中尤为显著。在`command`类和`file`类中，用户需要提供查询参数`commands`和`srcs`, `base_dir`. 其中`commands`为需要执行的所有指令，`srcs`为需要拷贝的所有文件，`base_dir`为拷贝文件后的目标目录，如不指定则默认为当前目录。
