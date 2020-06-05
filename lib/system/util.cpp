#include "util.h"

#include <string>
#include <unistd.h>

namespace libzinlidac {
    namespace system {
    long int get_result_using_sysconf(int __name, std::string &&target) {
        long int result = sysconf(__name);
        if (result >= 0) {
            return result;
        } else {
            throw SysconfError(std::move(target));
        }
    }
    }
}