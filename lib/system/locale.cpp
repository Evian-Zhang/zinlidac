#include "system.h"

#include <string>
#include <locale>
#ifdef __linux__
#include <langinfo.h>
#endif

namespace libzinlidac {
std::string get_locale() {
    return std::locale().name();
}

std::string get_codeset() {
    // No errors are defined
    return std::string(nl_langinfo(CODESET));
}
}