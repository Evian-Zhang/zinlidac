from graphene import ObjectType, String, Int

import libzinlidacpy

class Memory(ObjectType):
    virtual_memory_page_size = Int()
    physical_memory_page_size = Int()
    available_physical_memory_pages_number = Int()
    shared_memory_page_size = Int()
    swap_space_size = Int()
    meminfo = String()

    def resolve_virtual_memory_page_size(parent, info):
        return libzinlidacpy.get_virtual_memory_page_size();

    def resolve_physical_memory_page_size(parent, info):
        return libzinlidacpy.get_physical_memory_page_size();

    def resolve_available_physical_memory_pages_number(parent, info):
        return libzinlidacpy.get_available_physical_memory_pages_number();

    def resolve_shared_memory_page_size(parent, info):
        return libzinlidacpy.get_shared_memory_page_size();

    def resolve_swap_space_size(parent, info):
        return libzinlidacpy.get_swap_space_size();

    def resolve_meminfo(parent, info):
        return libzinlidacpy.get_meminfo();
