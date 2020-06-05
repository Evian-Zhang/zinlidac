from graphene import ObjectType, String

import libzinlidacpy

class OperatingSystem(ObjectType):
    system_name = String()
    system_release_level = String()
    system_release_version_level = String()

    def resolve_system_name(parent, info):
        return libzinlidacpy.get_system_name()

    def resolve_system_release_level(parent, info):
        return libzinlidacpy.get_system_release_level()

    def resolve_system_release_version_level(parent, info):
        return libzinlidacpy.get_system_release_version_level()
