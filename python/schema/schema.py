from graphene import ObjectType, String, Field, List

import schema.hardware.hardware as hardware_module
import schema.system.system as system_module
import schema.command as command_module
import schema.file as file_module

class Query(ObjectType):
    hardware = Field(hardware_module.Hardware)
    system = Field(system_module.System)
    command = List(command_module.Command, commands=List(String, required=True))
    file = List(file_module.File, srcs=List(String, required=True), base_dir=String())

    def resolve_hardware(parent, info):
        return hardware_module.Hardware()

    def resolve_system(parent, info):
        return system_module.System()

    def resolve_command(parent, info, commands):
        return list(map(lambda command_str: command_module.Command(command_str), commands))

    def resolve_file(parent, info, srcs, base_dir=None):
        if not base_dir:
            base_dir = "."
        return list(map(lambda file_path: file_module.File(file_path, base_dir), srcs))
