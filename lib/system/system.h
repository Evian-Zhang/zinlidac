#include <string>
#include <vector>

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

// --------------------------- User ---------------------------
struct LoginUserInfo {
    std::string name;
    unsigned int uid;
};
struct UserInfo {
    std::string name;
    unsigned int uid;
    unsigned int groupid;
    std::string full_name;
    std::string home_directory;
    std::string login_shell;
};
// get user logged in on the controlling terminal of the process
// throws a `SpecialError` if name of the user logged in cannot be determined
LoginUserInfo get_associated_user();
// get users list
// throws a `FileReadError` if cannot open /etc/passwd
// throws a `SpecialError` if cannot parse uid or groupid to integer
std::vector<UserInfo> get_users();

// --------------------------- Process ---------------------------
struct ProcessInfo {
    int ppid; // pid of parent process
    unsigned long long utime; // user-mode CPU time accumulated by process
    unsigned long long stime; // kernel-mode CPU time accumulated by process
    long long unsigned int start_time; // start time of process -- seconds since 1970
    long priority; // kernel scheduling priority
    long nice; // standard unix nice level of process
    unsigned long vm_size; // total virtual memory as kb
    unsigned long vm_resident; // resident non-swapped memory as kb
    unsigned long vm_share; // shared memory as kb
    std::vector<std::string> environment_variables; // environment string vector
    std::vector<std::string> cmdline_arguments; // command line string vector
    std::string user; // real user name
    std::string basename; // basename of executable file in call to exec(2)
    int threads_number; // number of threads, or 0 if no clue
};
// get list of environment variables
std::vector<std::string> get_environment_variables();
// get info of current running processes
// throws a `SpecialError` if cannot read /proc
std::vector<ProcessInfo> get_processes();
}