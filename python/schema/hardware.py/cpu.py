from graphene import ObjectType, String, Int, Schema, Field

class CPU(ObjectType):
    cpus_number = Int()

    def resolve_cpus_number(parent, info):
        return 1