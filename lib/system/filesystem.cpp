#include "system.h"
#include "util.h"

#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#endif

namespace libzinlidac {
// throws `POSIXMacroError` both if no limit for path length
long int get_path_max() {
    #ifdef __linux__
    // see https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_31.html
    #ifdef PATH_MAX
    return PATH_MAX;
    #else
    throw system::POSIXMacroError("PATH_MAX");
    #endif
    #endif
}

// throws `POSIXMacroError` both if no limit for filename length
long int get_filename_max() {
    #ifdef __linux__
    // see https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_31.html
    #ifdef FILENAME_MAX
    return FILENAME_MAX;
    #else
    throw system::POSIXMacroError("FILENAME_MAX");
    #endif
    #endif
}
}