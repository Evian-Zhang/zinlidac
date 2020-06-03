#include <string>
#ifdef __linux__
#include <sys/utsname.h>
#endif

namespace zinlidac {
std::string get_system_name() noexcept {
    #ifdef __linux__
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.sysname);
    #endif
}

std::string get_system_release_level() noexcept {
    #ifdef __linux__
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.release);
    #endif
}

std::string get_system_release_version_level() noexcept {
    #ifdef __linux__
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.version);
    #endif
}
}
