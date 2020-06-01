#include "hardware.h"
#include "util.h"

#include <string>
#include <fstream>
#include <stdlib.h>
#ifdef __linux__
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#endif

namespace libzinlidac {
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_cpus_number() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_NPROCESSORS_ONLN, "_SC_NPROCESSORS_ONLN");
    #endif
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_cpu_clock_ticks_per_time() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_NPROCESSORS_ONLN, "_SC_NPROCESSORS_ONLN");
    #endif
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_word_bit() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_NPROCESSORS_ONLN, "_SC_NPROCESSORS_ONLN");
    #endif
}

// throws a `SysinfoError` if cannot get system information
long get_boot_time() {
    #ifdef __linux__
    struct sysinfo *info = (struct sysinfo*)malloc(sizeof(struct sysinfo));
    try {
        sysinfo(info);
    } catch(...) {
        free(info);
        throw hardware::SysinfoError();
    }
    long boot_time = info->uptime;
    free(info);
    return boot_time;
    #endif
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
}
