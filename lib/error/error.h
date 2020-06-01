#ifndef LIBZINLIDAC_ERROR_H
#define LIBZINLIDAC_ERROR_H

#include <string>
namespace libzinlidac {
    class Error {
    public:
        std::string description();
    };
}

#endif
