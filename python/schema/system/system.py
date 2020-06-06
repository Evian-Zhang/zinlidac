from graphene import ObjectType, Field

import schema.system.operating_system as operating_system_module
import schema.system.filesystem as filesystem_module
import schema.system.time as time_module
import schema.system.locale as locale_module
import schema.system.user as user_module
import schema.system.process as process_module
import schema.system.network as network_module

class System(ObjectType):
    operating_system = Field(operating_system_module.OperatingSystem)
    filesystem = Field(filesystem_module.Filesystem)
    time = Field(time_module.Time)
    locale = Field(locale_module.Locale)
    user = Field(user_module.User)
    process = Field(process_module.Process)
    network = Field(network_module.Network)

    def resolve_operating_system(parent, info):
        return operating_system_module.OperatingSystem()

    def resolve_filesystem(parent, info):
        return filesystem_module.Filesystem()

    def resolve_time(parent, info):
        return time_module.Time()

    def resolve_locale(parent, info):
        return locale_module.Locale()

    def resolve_user(parent, info):
        return user_module.User()

    def resolve_process(parent, info):
        return process_module.Process()

    def resolve_network(parent, info):
        return network_module.Network()
