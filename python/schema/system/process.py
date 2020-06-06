from graphene import ObjectType, Int, String, Field, List

import libzinlidacpy

class ProcessInfo(ObjectType):
    ppid = Int()
    utime = Int()
    stime = Int()
    start_time = Int()
    priority = Int()
    nice = Int()
    vm_size = Int()
    vm_resident = Int()
    vm_share = Int()
    environment_variables = List(String)
    cmdline_arguments = List(String)
    user = String()
    basename = String()
    threads_number = Int()

    def __init__(self ,libProcessInfo):
        self.libProcessInfo = libProcessInfo

    def resolve_ppid(parent, info):
        return parent.libProcessInfo.ppid

    def resolve_utime(parent, info):
        return parent.libProcessInfo.utime

    def resolve_stime(parent, info):
        return parent.libProcessInfo.stime

    def resolve_start_time(parent, info):
        return parent.libProcessInfo.start_time

    def resolve_priority(parent, info):
        return parent.libProcessInfo.priority

    def resolve_nice(parent, info):
        return parent.libProcessInfo.nice

    def resolve_vm_size(parent, info):
        return parent.libProcessInfo.vm_size

    def resolve_vm_resident(parent, info):
        return parent.libProcessInfo.vm_resident

    def resolve_vm_share(parent, info):
        return parent.libProcessInfo.vm_share

    def resolve_environment_variables(parent, info):
        return parent.libProcessInfo.environment_variables

    def resolve_cmdline_arguments(parent, info):
        return parent.libProcessInfo.cmdline_arguments

    def resolve_user(parent, info):
        return parent.libProcessInfo.user

    def resolve_basename(parent, info):
        return parent.libProcessInfo.basename

    def resolve_threads_number(parent, info):
        return parent.libProcessInfo.threads_number

class Process(ObjectType):
    environment_variables = List(String)
    processes = List(ProcessInfo)
    path_environment = String

    def resolve_environment_variables(parent, info):
        return libzinlidacpy.get_environement_variables()

    def resolve_processes(parent, info):
        return list(map(lambda libProcessInfo: ProcessInfo(libProcessInfo), libzinlidacpy.get_processes()))

    def resolve_path_environment(parent, info):
        return libzinlidacpy.get_path_environment()
