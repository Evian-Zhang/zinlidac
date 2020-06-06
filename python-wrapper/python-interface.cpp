#include "lib/error/error.h"
#include "lib/hardware/hardware.h"
#include "lib/system/system.h"

#include <string>
#include <vector>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

void exception_translator(libzinlidac::Error const &error) {
    PyErr_SetString(PyExc_UserWarning, error.description().c_str());
}

BOOST_PYTHON_MODULE(libzinlidacpy) {
    using namespace boost::python;
    register_exception_translator<libzinlidac::Error>(exception_translator);

    // --------------------------- hardware ---------------------------
    // cpu
    def("get_cpus_number", libzinlidac::get_cpus_number);
    def("get_cpu_clock_ticks_per_time", libzinlidac::get_cpu_clock_ticks_per_time);
    def("get_word_bit", libzinlidac::get_word_bit);
    def("get_boot_time", libzinlidac::get_boot_time);
    def("get_cpuinfo", libzinlidac::get_cpuinfo);
    def("get_cpu_stat", libzinlidac::get_cpu_stat);
    def("get_hardware_type", libzinlidac::get_hardware_type);

    // cache
    def("get_level1_instruction_cache_size", libzinlidac::get_level1_instruction_cache_size);
    def("get_level1_data_cache_size", libzinlidac::get_level1_data_cache_size);
    def("get_level2_cache_size", libzinlidac::get_level2_cache_size);
    def("get_level3_cache_size", libzinlidac::get_level3_cache_size);
    def("get_level4_cache_size", libzinlidac::get_level4_cache_size);

    // memory
    def("get_virtual_memory_page_size", libzinlidac::get_virtual_memory_page_size);
    def("get_physical_memory_page_size", libzinlidac::get_physical_memory_page_size);
    def("get_available_physical_memory_pages_number", libzinlidac::get_available_physical_memory_pages_number);
    def("get_shared_memory_size", libzinlidac::get_shared_memory_size);
    def("get_swap_space_size", libzinlidac::get_swap_space_size);
    def("get_meminfo", libzinlidac::get_meminfo);

    // --------------------------- system ---------------------------
    // operating_system
    def("get_system_name", libzinlidac::get_system_name);
    def("get_system_release_level", libzinlidac::get_system_release_level);
    def("get_system_release_version_level", libzinlidac::get_system_release_version_level);

    // filesystem
    class_<libzinlidac::FilesystemInfo>("FilesystemInfo")
        .def_readonly("name", &libzinlidac::FilesystemInfo::name)
        .def_readonly("dir", &libzinlidac::FilesystemInfo::dir)
        .def_readonly("mount_type", &libzinlidac::FilesystemInfo::mount_type)
        .def_readonly("mount_options", &libzinlidac::FilesystemInfo::mount_options);
    class_<std::vector<libzinlidac::FilesystemInfo> >("FilesystemInfoVec")
        .def(vector_indexing_suite<std::vector<libzinlidac::FilesystemInfo> >());
    def("get_filesystem_capacity", libzinlidac::get_filesystem_capacity);
    def("get_filesystem_free_size", libzinlidac::get_filesystem_free_size);
    def("get_filesystem_type", libzinlidac::get_filesystem_type);
    def("get_current_mounted_filesystems", libzinlidac::get_current_mounted_filesystems);
    def("get_configured_mounted_filesystems", libzinlidac::get_configured_mounted_filesystems);
}
