#include "lib/hardware/hardware.h"
using namespace libzinlidac;

#include "gtest/gtest.h"

TEST(hardware_test, get_cpus_number) {
    EXPECT_NE(get_cpus_number(), -1);
}