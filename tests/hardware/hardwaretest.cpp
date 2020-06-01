#include "lib/hardware/hardware.h"
using namespace libzinlidac;

#include "gtest/gtest.h"

TEST(hardware_test, get_cpus_number) {
    EXPECT_NE(get_cpus_number(), -1);
}

TEST(hardware_test, get_cpu_clock_ticks_per_time) {
    EXPECT_NE(get_cpu_clock_ticks_per_time(), -1);
}

TEST(hardware_test, get_word_bit) {
    EXPECT_NE(get_word_bit(), -1);
}

TEST(hardware_test, get_level1_instruction_cache_size) {
    EXPECT_NE(get_level1_instruction_cache_size(), -1);
}

TEST(hardware_test, get_level1_data_cache_size) {
    EXPECT_NE(get_level1_data_cache_size(), -1);
}

TEST(hardware_test, get_level2_cache_size) {
    EXPECT_NE(get_level2_cache_size(), -1);
}

TEST(hardware_test, get_level3_cache_size) {
    EXPECT_NE(get_level3_cache_size(), -1);
}

TEST(hardware_test, get_level4_cache_size) {
    EXPECT_NE(get_level4_cache_size(), -1);
}

TEST(hardware_test, get_virtual_memory_page_size) {
    EXPECT_NE(get_virtual_memory_page_size(), -1);
}

TEST(hardware_test, get_physical_memory_page_size) {
    EXPECT_NE(get_physical_memory_page_size(), -1);
}

TEST(hardware_test, get_available_physical_memory_pages_number) {
    EXPECT_NE(get_available_physical_memory_pages_number(), -1);
}
