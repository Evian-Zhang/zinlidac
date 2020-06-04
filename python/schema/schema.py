from graphene import ObjectType, String, Schema, Field

import hardware

class Query(ObjectType):
    hardware = Field(hardware.Hardware)