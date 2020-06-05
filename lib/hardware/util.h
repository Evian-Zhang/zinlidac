#ifndef LIBZINLIDAC_HARDWARE_UTIL_H
#define LIBZINLIDAC_HARDWARE_UTIL_H

#include "lib/error/error.h"

namespace libzinlidac {
    namespace hardware {
    class SysconfError : public Error {
    private:
        const std::string target;
    public:
        SysconfError(std::string &&target) : target(target) { }

        virtual std::string description() const {
            return std::string("Cannot access ") + this->target;
        }
    };

    class FileReadError : public Error {
    private:
        const std::string file_path;
    public:
        FileReadError(std::string &&file_path) : file_path(file_path) { }

        virtual std::string description() const {
            return std::string("Cannot read file ") + this->file_path;
        }
    };

    long int get_result_using_sysconf(int __name, std::string &&target);
    }
}

#endif