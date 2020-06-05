from graphene import ObjectType, String, Int

import libzinlidacpy

class Cache(ObjectType):
    level1_instruction_cache_size = Int()
    level1_data_cache_size = Int()
    level2_cache_size = Int()
    level3_cache_size = Int()
    level4_cache_size = Int()

    def resolve_level1_instruction_cache_size(parent, info):
        return libzinlidacpy.get_level1_instruction_cache_size()

    def resolve_level1_data_cache_size(parent, info):
        return libzinlidacpy.get_level1_instruction_cache_size()

    def resolve_level2_cache_size(parent, info):
        return libzinlidacpy.get_level2_cache_size()

    def resolve_level3_cache_size(parent, info):
        return libzinlidacpy.get_level3_cache_size()

    def resolve_level4_cache_size(parent, info):
        return libzinlidacpy.get_level4_cache_size()
