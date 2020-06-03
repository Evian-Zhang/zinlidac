#include "system.h"
#include "util.h"

#include <vector>
#include <string>
#include <cstring>
#ifdef __linux__
// need `apt install libprocps-dev`
#include <proc/readproc.h>
#endif

// see `man environ`
extern char **environ;

namespace libzinlidac {
std::vector<std::string> get_environment_variables() noexcept {
    #ifdef __linux__
    std::vector<std::string> environment_variables;
    int index = 0;
    char *env;
    while (::environ != NULL && (env = ::environ[index++]) != NULL) {
        environment_variables.push_back(std::string(env));
    }
    return environment_variables;
    #endif
}

// throws a `SpecialError` if cannot read /proc
std::vector<ProcessInfo> get_processes() {
    #ifdef __linux__
    std::vector<ProcessInfo> process_infos;
    PROCTAB *proc_tab = openproc(PROC_FILLMEM | PROC_FILLCOM | PROC_FILLENV | PROC_FILLUSR | PROC_FILLARG | PROC_FILLSTAT | PROC_FILLSTATUS);
    if (proc_tab == NULL) {
        throw system::SpecialError("Cannot read /proc");
    }
    proc_t process_info;
    // From proc/readproc.h:
    //
    // Alternatively, you may provide your own reuseable buffer address
    // in which case that buffer *MUST* be initialized to zero one time
    // only before first use.
    memset(&process_info, 0, sizeof(process_info));
    while (readproc(proc_tab, &process_info) != NULL) {
        std::vector<std::string> environment_variables, cmdline_arguments;
        int index = 0;
        char *var;
        while (process_info.environ != NULL && (var = process_info.environ[index++]) != NULL) {
            environment_variables.push_back(std::string(var));
        }
        index = 0;
        while (process_info.cmdline != NULL && (var = process_info.cmdline[index++]) != NULL) {
            cmdline_arguments.push_back(std::string(var));
        }
        process_infos.push_back((ProcessInfo){
            .ppid = process_info.ppid,
            .utime = process_info.utime,
            .stime = process_info.stime,
            .start_time = process_info.start_time,
            .priority = process_info.priority,
            .nice = process_info.nice,
            .vm_size = process_info.vm_size,
            .vm_resident = process_info.vm_rss,
            .vm_share = process_info.vm_rss_shared,
            .environment_variables = std::move(environment_variables),
            .cmdline_arguments = std::move(cmdline_arguments),
            .user = std::string(process_info.ruser),
            .basename = std::string(process_info.cmd),
            .threads_number = process_info.nlwp
        });
    }
    return process_infos;
    #endif
}
}
