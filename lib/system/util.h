#ifndef LIBZINLIDAC_SYSTEM_UTIL_H
#define LIBZINLIDAC_SYSTEM_UTIL_H

#include "lib/error/error.h"

#include <string>
#ifdef __linux__
#include <unistd.h>
#endif

namespace libzinlidac {
    namespace system {
    class SysconfError : Error {
    private:
        std::string target;
    public:
        SysconfError(std::string &&target) {
            this->target = target;
        }

        std::string description() {
            return std::string("Cannot access ") + this->target;
        }
    };

    class POSIXMarcoError : Error {
    private:
        std::string macro_name;
    public:
        POSIXMarcoError(std::string &&macro_name) {
            this->macro_name = macro_name;
        }

        std::string description() {
            return std::string("Cannot get value of POSIX macro ") + this->macro_name;
        }
    };

    #ifdef __linux__
    long int get_result_using_sysconf(int __name, std::string &&target) {
        long int result = sysconf(__name);
        if (result >= 0) {
            return result;
        } else {
            throw SysconfError(std::move(target));
        }
    }
    #endif
    }
}

#endif
