from graphene import ObjectType, String, Schema, Field

class Hardware(ObjectType):
    cpu = Field(CPU)

    def resolve_cpu(parent, info):
        return CPU()