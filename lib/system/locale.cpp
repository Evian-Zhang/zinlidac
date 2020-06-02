#include "system.h"

#include <string>
#include <locale>
#ifdef __linux__
#include <langinfo.h>
#endif

namespace libzinlidac {
std::string get_locale() {
    #ifdef __linux__
    return std::locale().name();
    #endif
}

std::string get_codeset() {
    #ifdef __linux__
    // No errors are defined
    return std::string(nl_langinfo(CODESET));
    #endif
}
}