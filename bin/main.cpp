#include "lib/error/error.h"
#include "lib/hardware/hardware.h"
#include "lib/system/system.h"

#include <iostream>

int main() {
    try {
        for (auto result : libzinlidac::get_lsof__Ua())
            std::cout << result.command << "\t" << result.user << "\t" << result.type << std::endl;
    } catch (const libzinlidac::Error &e) {
        std::cout << e.description() << std::endl;
    }
    return 0;
}