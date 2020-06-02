#include <string>
#ifdef __linux__
#include <sys/utsname.h>
#endif

namespace zinlidac {
std::string get_system_name() {
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.sysname);
}

std::string get_system_release_level() {
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.release);
}

std::string get_system_release_version_level() {
    struct utsname unix_name;
    // No errors are defined
    uname(&unix_name);
    return std::string(unix_name.version);
}
}
