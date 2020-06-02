#include "lib/system/system.h"
#include <iostream>
int main() {
    for (auto env : libzinlidac::get_environment_variables())
        std::cout << env;
    return 0;
}