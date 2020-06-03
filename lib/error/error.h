#ifndef LIBZINLIDAC_ERROR_H
#define LIBZINLIDAC_ERROR_H

#include <string>
namespace libzinlidac {
    class Error {
    public:
        virtual std::string description() const {
            return std::string("");
        }
    };
}

#endif
