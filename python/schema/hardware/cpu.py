from graphene import ObjectType, String, Int

import libzinlidacpy

class CPU(ObjectType):
    cpus_number = Int()
    cpu_clock_ticks_per_time = Int()
    word_bit = Int()
    boot_time = Int()
    cpuinfo = String()
    cpu_stat = String()
    hardware_type = String()

    def resolve_cpus_number(parent, info):
        return libzinlidacpy.get_cpus_number()

    def resolve_cpu_clock_ticks_per_time(parent, info):
        return libzinlidacpy.get_cpu_clock_ticks_per_time()
    
    def resolve_word_bit(parent, info):
        return libzinlidacpy.get_word_bit()
    
    def resolve_boot_time(parent, info):
        return libzinlidacpy.get_boot_time()

    def resolve_cpuinfo(parent, info):
        return libzinlidacpy.get_cpuinfo()

    def resolve_cpu_stat(parent, info):
        return libzinlidacpy.get_cpu_stat()

    def resolve_hardware_type(parent, info):
        return libzinlidacpy.get_hardware_type()
