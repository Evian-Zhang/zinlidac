#include "system.h"
#include "util.h"

#include <string>
#include <fstream>
#include <sys/time.h>

namespace libzinlidac {
long int get_current_time() noexcept {
    struct timeval tp;
    // NO errors are defined
    //
    // From `man gettimeofday`:
    //
    // The  use  of the timezone structure is obsolete; the tz argument should
    // normally be specified as NULL.  (See NOTES below.)
    //
    // Under Linux, there are some peculiar "warp clock" semantics  associated
    // with  the  settimeofday()  system call if on the very first call (after
    // booting) that has a non-NULL tz argument, the tv argument is  NULL  and
    // the  tz_minuteswest  field is nonzero.  (The tz_dsttime field should be
    // zero for this case.)  In such a case it is assumed that the CMOS  clock
    // is  on  local time, and that it has to be incremented by this amount to
    // get UTC system time.  No doubt it is a bad idea to use this feature.
    gettimeofday(&tp, NULL);
    return tp.tv_sec;
}

// throws a `FileReadError` if cannot open /etc/timezone
std::string get_timezone() {
    auto timezone_file = std::ifstream("/etc/timezone");
    if (timezone_file.is_open()) {
        // see https://stackoverflow.com/a/2912614/10005095
        return std::string((std::istreambuf_iterator<char>(timezone_file)), (std::istreambuf_iterator<char>()));
    } else {
        throw system::FileReadError("/etc/timezone");
    }
}
}