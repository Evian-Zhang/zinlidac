from graphene import ObjectType, Field

import schema.hardware.cpu as cpu_module
import schema.hardware.cache as cache_module
import schema.hardware.memory as memory_module

class Hardware(ObjectType):
    cpu = Field(cpu_module.CPU)
    cache = Field(cache_module.Cache)
    memory = Field(memory_module.Memory)

    def resolve_cpu(parent, info):
        return cpu_module.CPU()

    def resolve_cache(parent, info):
        return cache_module.Cache()

    def resolve_memory(parent, info):
        return memory_module.Memory()
