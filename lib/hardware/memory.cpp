#include "hardware.h"
#include "util.h"

#include <string>
#include <stdlib.h>
#ifdef __linux__
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#endif

namespace libzinlidac {
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_virtual_memory_page_size() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_PAGESIZE, "_SC_PAGESIZE");
    #endif
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_physical_memory_page_size() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_PHYS_PAGES, "_SC_PHYS_PAGES");
    #endif
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_available_physical_memory_pages_number() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_AVPHYS_PAGES, "_SC_AVPHYS_PAGES");
    #endif
}

// throws a `SysinfoError` if cannot get system information
unsigned long get_shared_memory_size() {
    #ifdef __linux__
    struct sysinfo *info = (struct sysinfo*)malloc(sizeof(struct sysinfo));
    try {
        sysinfo(info);
    } catch(...) {
        free(info);
        throw hardware::SysinfoError();
    }
    unsigned long shared_memory_size = info->sharedram;
    free(info);
    return shared_memory_size;
    #endif
}

// throws a `SysinfoError` if cannot get system information
unsigned long get_swap_space_size() {
    #ifdef __linux__
    struct sysinfo *info = (struct sysinfo*)malloc(sizeof(struct sysinfo));
    try {
        sysinfo(info);
    } catch(...) {
        free(info);
        throw hardware::SysinfoError();
    }
    unsigned long swap_space_size = info->totalswap;
    free(info);
    return swap_space_size;
    #endif
}
}