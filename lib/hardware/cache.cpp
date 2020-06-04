#include "hardware.h"
#include "util.h"

#include <string>
#include <unistd.h>

namespace libzinlidac {
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level1_instruction_cache_size() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_LEVEL1_ICACHE_SIZE, "_SC_LEVEL1_ICACHE_SIZE");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level1_data_cache_size() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_LEVEL1_DCACHE_SIZE, "_SC_LEVEL1_DCACHE_SIZE");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level2_cache_size() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_LEVEL2_CACHE_SIZE, "_SC_LEVEL2_CACHE_SIZE");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level3_cache_size() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_LEVEL3_CACHE_SIZE, "_SC_LEVEL3_CACHE_SIZE");
}

// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level4_cache_size() {
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return hardware::get_result_using_sysconf(_SC_LEVEL4_CACHE_SIZE, "_SC_LEVEL4_CACHE_SIZE");
}
}