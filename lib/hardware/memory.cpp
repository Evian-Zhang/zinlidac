#include "hardware.h"
#include "util.h"

#include <string>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>

namespace libzinlidac {
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_virtual_memory_page_size() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_PAGESIZE, "_SC_PAGESIZE");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_physical_memory_page_size() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_PHYS_PAGES, "_SC_PHYS_PAGES");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_available_physical_memory_pages_number() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_AVPHYS_PAGES, "_SC_AVPHYS_PAGES");
}

unsigned long get_shared_memory_size() noexcept {
    struct sysinfo info;
    // return -1 if `&info` is not a valid address, which is impossible
    sysinfo(&info);
    unsigned long shared_memory_size = info.sharedram;
    return shared_memory_size;
}

unsigned long get_swap_space_size() noexcept {
    struct sysinfo info;
    // return -1 if `&info` is not a valid address, which is impossible
    sysinfo(&info);
    unsigned long swap_space_size = info.totalswap;
    return swap_space_size;
}

// throws a `FileReadError` if cannot open /proc/meminfo
std::string get_meminfo() {
    auto meminfo_file = std::ifstream("/proc/meminfo");
    if (meminfo_file.is_open()) {
        // see https://stackoverflow.com/a/2912614/10005095
        return std::string((std::istreambuf_iterator<char>(meminfo_file)), (std::istreambuf_iterator<char>()));
    } else {
        throw hardware::FileReadError("/proc/meminfo");
    }
}
}