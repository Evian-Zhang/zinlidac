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
    class_<std::vector<libzinlidac::FilesystemInfo> >("FilesystemInfoList")
        .def(vector_indexing_suite<std::vector<libzinlidac::FilesystemInfo> >());
    def("get_filesystem_capacity", libzinlidac::get_filesystem_capacity);
    def("get_filesystem_free_size", libzinlidac::get_filesystem_free_size);
    def("get_filesystem_type", libzinlidac::get_filesystem_type);
    def("get_current_mounted_filesystems", libzinlidac::get_current_mounted_filesystems);
    def("get_configured_mounted_filesystems", libzinlidac::get_configured_mounted_filesystems);

    // time
    def("get_current_time", libzinlidac::get_current_time);
    def("get_timezone", libzinlidac::get_timezone);

    // locale
    def("get_locale", libzinlidac::get_locale);
    def("get_codeset", libzinlidac::get_codeset);

    // user
    class_<libzinlidac::LoginUserInfo>("LoginUserInfo")
        .def_readonly("name", &libzinlidac::LoginUserInfo::name)
        .def_readonly("uid", &libzinlidac::LoginUserInfo::uid);
    class_<libzinlidac::UserInfo>("UserInfo")
        .def_readonly("name", &libzinlidac::UserInfo::name)
        .def_readonly("uid", &libzinlidac::UserInfo::uid)
        .def_readonly("groupid", &libzinlidac::UserInfo::groupid)
        .def_readonly("full_name", &libzinlidac::UserInfo::full_name)
        .def_readonly("home_directory", &libzinlidac::UserInfo::home_directory)
        .def_readonly("login_shell", &libzinlidac::UserInfo::login_shell);
    class_<libzinlidac::GroupInfo>("GroupInfo")
        .def_readonly("name", &libzinlidac::GroupInfo::name)
        .def_readonly("gid", &libzinlidac::GroupInfo::gid)
        .def_readonly("users", &libzinlidac::GroupInfo::users);
    class_<std::vector<std::string> >("GroupInfo_users")
        .def(vector_indexing_suite<std::vector<std::string> >());
    class_<std::vector<libzinlidac::UserInfo> >("UserInfoList")
        .def(vector_indexing_suite<std::vector<libzinlidac::UserInfo> >());
    class_<std::vector<libzinlidac::GroupInfo> >("GroupInfoList")
        .def(vector_indexing_suite<std::vector<libzinlidac::GroupInfo> >());
    def("get_associated_user", libzinlidac::get_associated_user);
    def("get_users", libzinlidac::get_users);
    def("get_groups", libzinlidac::get_groups);

    // process
    class_<libzinlidac::ProcessInfo>("ProcessInfo")
        .def_readonly("ppid", &libzinlidac::ProcessInfo::ppid)
        .def_readonly("utime", &libzinlidac::ProcessInfo::utime)
        .def_readonly("stime", &libzinlidac::ProcessInfo::stime)
        .def_readonly("start_time", &libzinlidac::ProcessInfo::start_time)
        .def_readonly("priority", &libzinlidac::ProcessInfo::priority)
        .def_readonly("nice", &libzinlidac::ProcessInfo::nice)
        .def_readonly("vm_size", &libzinlidac::ProcessInfo::vm_size)
        .def_readonly("vm_resident", &libzinlidac::ProcessInfo::vm_resident)
        .def_readonly("vm_share", &libzinlidac::ProcessInfo::vm_share)
        .def_readonly("environment_variables", &libzinlidac::ProcessInfo::environment_variables)
        .def_readonly("cmdline_arguments", &libzinlidac::ProcessInfo::cmdline_arguments)
        .def_readonly("user", &libzinlidac::ProcessInfo::user)
        .def_readonly("basename", &libzinlidac::ProcessInfo::basename)
        .def_readonly("threads_number", &libzinlidac::ProcessInfo::threads_number);
    class_<std::vector<libzinlidac::ProcessInfo> >("ProcessInfoList")
        .def(vector_indexing_suite<std::vector<libzinlidac::ProcessInfo> >());
    def("get_environment_variables", libzinlidac::get_environment_variables);
    def("get_processes", libzinlidac::get_processes);
    def("get_path_environment", libzinlidac::get_path_environment);

    // network
    class_<libzinlidac::NetworkInterfaceInfo>("NetworkInterfaceInfo")
        .def_readonly("name", &libzinlidac::NetworkInterfaceInfo::name)
        .def_readonly("network_family", &libzinlidac::NetworkInterfaceInfo::network_family)
        .def_readonly("address", &libzinlidac::NetworkInterfaceInfo::address);
    class_<libzinlidac::LsofIResult>("LsofIResult")
        .def_readonly("command", &libzinlidac::LsofIResult::command)
        .def_readonly("pid", &libzinlidac::LsofIResult::pid)
        .def_readonly("user", &libzinlidac::LsofIResult::user)
        .def_readonly("fd", &libzinlidac::LsofIResult::fd)
        .def_readonly("type", &libzinlidac::LsofIResult::type)
        .def_readonly("device", &libzinlidac::LsofIResult::device)
        .def_readonly("node", &libzinlidac::LsofIResult::node)
        .def_readonly("name", &libzinlidac::LsofIResult::name);
    class_<libzinlidac::LsofUaResult>("LsofUaResult")
        .def_readonly("command", &libzinlidac::LsofUaResult::command)
        .def_readonly("pid", &libzinlidac::LsofUaResult::pid)
        .def_readonly("user", &libzinlidac::LsofUaResult::user)
        .def_readonly("fd", &libzinlidac::LsofUaResult::fd)
        .def_readonly("type", &libzinlidac::LsofUaResult::type)
        .def_readonly("device", &libzinlidac::LsofUaResult::device)
        .def_readonly("name", &libzinlidac::LsofUaResult::name);
    class_<std::vector<libzinlidac::NetworkInterfaceInfo> >("NetworkInterfaceInfoList")
        .def(vector_indexing_suite<std::vector<libzinlidac::NetworkInterfaceInfo> >());
    class_<std::vector<libzinlidac::LsofIResult> >("LsofIResultList")
        .def(vector_indexing_suite<std::vector<libzinlidac::LsofIResult> >());
    class_<std::vector<libzinlidac::LsofUaResult> >("LsofUaResultList")
        .def(vector_indexing_suite<std::vector<libzinlidac::LsofUaResult> >());
    def("get_network_interfaces", libzinlidac::get_network_interfaces);
    def("get_lsof__i", libzinlidac::get_lsof__i);
    def("get_lsof__Ua", libzinlidac::get_lsof__Ua);
}
