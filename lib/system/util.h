#ifndef LIBZINLIDAC_SYSTEM_UTIL_H
#define LIBZINLIDAC_SYSTEM_UTIL_H

#include "lib/error/error.h"

#include <string>
#ifdef __linux__
#include <unistd.h>
#endif

namespace libzinlidac {
    namespace system {
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

    class StatvfsError : public Error {
    public:
        virtual std::string description() const {
            return std::string("Cannot determine using statvfs");
        }
    };

    class SpecialError : public Error {
    private:
        const std::string description_string;
    public:
        SpecialError(std::string &&description_string) : description_string(description_string) { }

        virtual std::string description() const { return this->description_string; }
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
