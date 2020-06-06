from graphene import ObjectType, String

import libzinlidacpy

class Locale(ObjectType):
    locale = String()
    codeset = String()

    def resolve_locale(parent, info):
        return libzinlidacpy.get_locale()

    def resolve_codeset(parent, info):
        return lbzinlidacpy.get_codeset()
        