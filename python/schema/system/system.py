from graphene import ObjectType, Field

import schema.system.operating_system as operating_system_module

class System(ObjectType):
    operating_system = Field(operating_system_module.OperatingSystem)

    def resolve_operating_system(parent, info):
        return operating_system_module.OperatingSystem()
