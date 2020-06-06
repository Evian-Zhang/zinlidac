from graphene import ObjectType, Field

import schema.system.operating_system as operating_system_module
import schema.system.filesystem as filesystem_module

class System(ObjectType):
    operating_system = Field(operating_system_module.OperatingSystem)
    filesystem = Field(filesystem_module.Filesystem)

    def resolve_operating_system(parent, info):
        return operating_system_module.OperatingSystem()

    def resolve_filesystem(parent, info):
        return filesystem_module.Filesystem()
