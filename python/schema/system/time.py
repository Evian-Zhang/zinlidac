from graphene import ObjectType, Int, String

import libzinlidacpy

class Time(ObjectType):
    current_time = Int()
    timezone = String()

    def resolve_current_time(parent, info):
        return libzinlidacpy.get_current_time()

    def resolve_timezone(parent, info):
        return libzinlidacpy.get_timezone()
        