#include "hardware.h"
#include "util.h"

#include <string>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>

namespace libzinlidac {
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_cpus_number() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_NPROCESSORS_ONLN, "_SC_NPROCESSORS_ONLN");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_cpu_clock_ticks_per_time() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_NPROCESSORS_ONLN, "_SC_NPROCESSORS_ONLN");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_word_bit() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_NPROCESSORS_ONLN, "_SC_NPROCESSORS_ONLN");
}

long get_boot_time() noexcept {
    struct sysinfo info;
    // return -1 if `&info` is not a valid address, which is impossible
    sysinfo(&info);
    long boot_time = info.uptime;
    return boot_time;
}

// throws a `FileReadError` if cannot open /proc/cpuinfo
std::string get_cpuinfo() {
    auto cpuinfo_file = std::ifstream("/proc/cpuinfo");
    if (cpuinfo_file.is_open()) {
        // see https://stackoverflow.com/a/2912614/10005095
        return std::string((std::istreambuf_iterator<char>(cpuinfo_file)), (std::istreambuf_iterator<char>()));
    } else {
        throw hardware::FileReadError("/proc/cpuinfo");
    }
}

// throws a `FileReadError` if cannot open /proc/stat
std::string get_cpu_stat() {
    auto cpustat_file = std::ifstream("/proc/stat");
    if (cpustat_file.is_open()) {
        // see https://stackoverflow.com/a/2912614/10005095
        return std::string((std::istreambuf_iterator<char>(cpustat_file)), (std::istreambuf_iterator<char>()));
    } else {
        throw hardware::FileReadError("/proc/stat");
    }
}

std::string get_hardware_type() noexcept {
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.machine);
}
}
