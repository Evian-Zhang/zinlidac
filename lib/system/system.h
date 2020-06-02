#include <string>

namespace libzinlidac {
// --------------------------- Operating system ---------------------------
// get name of this implementation of the operating system
std::string get_system_name();
// get current release level of this implementation
std::string get_system_release_level();
// get current version level of this release
std::string get_system_release_version_level();

// --------------------------- Filesystem ---------------------------

// --------------------------- Time ---------------------------
struct TimeSince1970 {
    long int seconds;
    long int milliseconds;
};

// get current time in seconds and milliseconds
TimeSince1970 get_current_time();
// get current timezone
// throws a `FileReadError` if cannot open /etc/timezone
std::string get_timezone();

// --------------------------- Locale ---------------------------
// get name of current global locale
std::string get_locale();
// get name of current codeset
std::string get_codeset();
}