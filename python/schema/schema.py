from graphene import ObjectType, Field

import schema.hardware.hardware as hardware_module
import schema.system.system as system_module

class Query(ObjectType):
    hardware = Field(hardware_module.Hardware)
    system = Field(system_module.System)

    def resolve_hardware(parent, info):
        return hardware_module.Hardware()

    def resolve_system(parent, info):
        return system_module.System()
