#include "hardware.h"

#include <unistd.h>

namespace libzinlidac {
// return -1 both if the implementation does not impose a limit, and in case of an error
long int get_cpus_number() {
    #ifdef __linux__
    // https://www.gnu.org/software/libc/manual/html_node/Sysconf.html
    return sysconf(_SC_NPROCESSORS_ONLN);
    #endif
}
}