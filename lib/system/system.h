#include <string>
#include <vector>

namespace libzinlidac {
// --------------------------- Operating system ---------------------------
// get name of this implementation of the operating system
std::string get_system_name() noexcept;
// get current release level of this implementation
std::string get_system_release_level() noexcept;
// get current version level of this release
std::string get_system_release_version_level() noexcept;

// --------------------------- Filesystem ---------------------------
struct FilesystemInfo {
public:
    std::string name;
    std::string dir; // filesystem path prefix
    std::string mount_type;
    std::string mount_options;

    bool operator==(const FilesystemInfo &rhs) const {
        return this->name == rhs.name && this->dir == rhs.dir && this->mount_type == rhs.mount_type && this->mount_options == rhs.mount_options;
    }
};

// get capacity of /
// throws a `StatvfsError` if statvfs fails
unsigned long get_filesystem_capacity();
// get free space size of /
// throws a `StatvfsError` if statvfs fails
unsigned long get_filesystem_free_size();
// get name of filesystem type
// throws a `SpecialError` if type is unknown or statfs fails
std::string get_filesystem_type();
// get information of current mounted filesystems
// throws a `FileReadError` if cannot open /etc/mtab
std::vector<FilesystemInfo> get_current_mounted_filesystems();
// get information of filesystems that is configured to be mounted by the installation or sysadmin
// throws a `FileReadError` if cannot open /etc/fstab
std::vector<FilesystemInfo> get_configured_mounted_filesystems();
// --------------------------- Time ---------------------------
struct TimeSince1970 {
    long int seconds;
    long int milliseconds;
};

// get current time in seconds and milliseconds
long int get_current_time() noexcept;
// get current timezone
// throws a `FileReadError` if cannot open /etc/timezone
std::string get_timezone();

// --------------------------- Locale ---------------------------
// get name of current global locale
std::string get_locale();
// get name of current codeset
std::string get_codeset();

// --------------------------- User ---------------------------
struct LoginUserInfo {
    std::string name;
    unsigned int uid;
};
struct UserInfo {
public:
    std::string name;
    unsigned int uid;
    unsigned int groupid;
    std::string full_name;
    std::string home_directory;
    std::string login_shell;

    bool operator==(const UserInfo &rhs) const {
        return this->name == rhs.name && this->uid == rhs.uid && this->groupid == rhs.groupid && this->full_name == rhs.full_name && this->home_directory == rhs.home_directory && this->login_shell == rhs.login_shell;
    }
};
struct GroupInfo {
public:
    std::string name;
    unsigned int gid;
    std::vector<std::string> users;

    bool operator==(const GroupInfo &rhs) const {
        return this->name == rhs.name && this->gid == rhs.gid && this->users == rhs.users;
    }
};
// get user logged in on the controlling terminal of the process
// throws a `SpecialError` if name of the user logged in cannot be determined
LoginUserInfo get_associated_user();
// get users list
// throws a `FileReadError` if cannot open /etc/passwd
// throws a `SpecialError` if cannot parse uid or groupid to integer
std::vector<UserInfo> get_users();
// get groups list
// throws a `FileReadError` if cannot open /etc/group
// throws a `SpecialError` if cannot parse gid to integer
std::vector<GroupInfo> get_groups();

// --------------------------- Process ---------------------------
struct ProcessInfo {
public:
    int ppid; // pid of parent process
    unsigned long long utime; // user-mode CPU time accumulated by process
    unsigned long long stime; // kernel-mode CPU time accumulated by process
    long long unsigned int start_time; // start time of process -- seconds since 1970
    long priority; // kernel scheduling priority
    long nice; // standard unix nice level of process
    unsigned long vm_size; // total virtual memory as kb
    unsigned long vm_resident; // resident non-swapped memory as kb
    unsigned long vm_share; // shared memory as kb
    std::vector<std::string> environment_variables; // environment string vector
    std::vector<std::string> cmdline_arguments; // command line string vector
    std::string user; // real user name
    std::string basename; // basename of executable file in call to exec(2)
    int threads_number; // number of threads, or 0 if no clue

    bool operator==(const ProcessInfo &rhs) const {
        return this->ppid == rhs.ppid && this->utime == rhs.utime && this->stime == rhs.stime && this->start_time == rhs.start_time && this->priority == rhs.priority && this->nice == rhs.nice && this->vm_size == rhs.vm_size && this->vm_resident == rhs.vm_resident && this->environment_variables == rhs.environment_variables && this->cmdline_arguments == rhs.cmdline_arguments && this->user == rhs.user && this->basename == rhs.basename && this->threads_number == rhs.threads_number;
    }
};
// get list of environment variables
std::vector<std::string> get_environment_variables() noexcept;
// get info of current running processes
// throws a `SpecialError` if cannot read /proc
std::vector<ProcessInfo> get_processes();
// get PATH environment variable
// throws a `SepecialError` if cannot find PATH
std::string get_path_environment();

// --------------------------- Network ---------------------------
struct NetworkInterfaceInfo {
public:
    std::string name;
    std::string network_family;
    std::string address;

    bool operator==(const NetworkInterfaceInfo &rhs) const {
        return this->name == rhs.name && this->network_family == rhs.network_family && this->address == rhs.address;
    }
};
struct LsofIResult {
public:
    std::string command;
    unsigned int pid;
    std::string user;
    std::string fd;
    std::string type;
    std::string device;
    std::string node;
    std::string name;

    bool operator==(const LsofIResult &rhs) const {
        return this->command == rhs.command && this->pid == rhs.pid && this->user == rhs.user && this->fd == rhs.fd && this->type == rhs.type && this->device == rhs.device && this->node == rhs.node && this->name == rhs.name;
    }
};
struct LsofUaResult {
public:
    std::string command;
    unsigned int pid;
    std::string user;
    std::string fd;
    std::string type;
    std::string device;
    std::string name;

    bool operator==(const LsofUaResult &rhs) const {
        return this->command == rhs.command && this->pid == rhs.pid && this->user == rhs.user && this->fd == rhs.fd && this->type == rhs.type && this->device == rhs.device && this->name == rhs.name;
    }
};
// get list of network interfaces
// throws a `SpecialError` if getifaddrs failed
std::vector<NetworkInterfaceInfo> get_network_interfaces();
// get the result of executing `lsof -i`, i.e. internet connection
// throws a SpecialError if failed to execute lsof -i
std::vector<LsofIResult> get_lsof__i();
// get the result of executing `lsof -Ua`, i.e. Unix domain socket
// throws a SpecialError if failed to execute lsof -Ua
std::vector<LsofUaResult> get_lsof__Ua();
}
