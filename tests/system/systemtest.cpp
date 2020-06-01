#include "lib/system/system.h"
using namespace libzinlidac;

#include "gtest/gtest.h"

TEST(system_test, get_posix1_version) {
    EXPECT_NE(get_posix1_version(), -1);
}

TEST(system_test, get_posix2_version) {
    EXPECT_NE(get_posix2_version(), -1);
}

TEST(system_test, get_path_max) {
    EXPECT_GE(get_path_max(), -1);
}

TEST(system_test, get_filename_max) {
    EXPECT_GE(get_filename_max(), -1);
}
