#include <string>
#include <sys/utsname.h>

namespace zinlidac {
std::string get_system_name() noexcept {
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.sysname);
}

std::string get_system_release_level() noexcept {
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.release);
}

std::string get_system_release_version_level() noexcept {
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.version);
}
}
