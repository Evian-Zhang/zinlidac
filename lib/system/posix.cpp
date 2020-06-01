#include "system.h"
#include "util.h"

#ifdef __linux__
#include <unistd.h>
#endif

namespace libzinlidac {
// return -1 both if the implementation does not impose a limit, and in case of an error
unsigned long get_posix1_version() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return system::get_result_using_sysconf(_SC_VERSION, "_SC_VERSION");
    #endif
}

// return -1 both if the implementation does not impose a limit, and in case of an error
unsigned long get_posix2_version() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return system::get_result_using_sysconf(_SC_2_VERSION, "_SC_2_VERSION");
    #endif
}
}