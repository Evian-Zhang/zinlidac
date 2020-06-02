#include <string>

namespace libzinlidac {
// --------------------------- CPU ---------------------------
// get number of active cpus
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_cpus_number();
// get number of clock ticks per second
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_cpu_clock_ticks_per_time();
// get number of bits in a variable of a register word
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_word_bit();
// get boot time in seconds
long get_boot_time();
// get content of /proc/cpuinfo
// throws a `FileReadError` if cannot open /proc/cpuinfo
std::string get_cpuinfo();
// get content of /proc/stat
// throws a `FileReadError` if cannot open /proc/stat
std::string get_cpu_stat();
// get name of the hardware type on which the system is running
std::string get_hardware_type();

// --------------------------- Cache ---------------------------
// get size of the Level 1 instruction cache
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level1_instruction_cache_size();
// get size of the Level 1 data cache
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level1_data_cache_size();
// get size of the Level 2 cache
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level2_cache_size();
// get size of the Level 3 cache
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level3_cache_size();
// get size of the Level 4 cache
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_level4_cache_size();

// --------------------------- Memory ---------------------------
// get size of virtual memory page
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_virtual_memory_page_size();
// get size of physical memory page
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_physical_memory_page_size();
// get number of available physical memory pages
// throws a `SysconfError` both if the implementation does not impose a limit, and in case of an error
unsigned long get_available_physical_memory_pages_number();
// get size of shared memory
unsigned long get_shared_memory_size();
// get size of swap space
unsigned long get_swap_space_size();
}
