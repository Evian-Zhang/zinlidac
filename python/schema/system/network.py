from graphene import ObjectType, Int, String, List

import libzinlidacpy

class NetworkInterface(ObjectType):
    name = String()
    network_family = String()
    address = String()

    def __init__(self, libNetworkInterface):
        self.libNetworkInterface = libNetworkInterface

    def resolve_name(parent, info):
        return parent.libNetworkInterface.name

    def resolve_network_family(parent, info):
        return parent.libNetworkInterface.network_family

    def resolve_address(parent, info):
        return parent.libNetworkInterface.address

class LsofIResult(ObjectType):
    command = String()
    pid = Int()
    user = String()
    fd = String()
    type = String()
    device = String()
    node = String()
    name = String()

    def __init__(self, libLsofIResult):
        self.libLsofIResult = libLsofIResult

    def resolve_command(parent, info):
        return parent.libLsofIResult.command

    def resolve_pid(parent, info):
        return parent.libLsofIResult.pid

    def resolve_user(parent, info):
        return parent.libLsofIResult.user

    def resolve_fd(parent, info):
        return parent.libLsofIResult.fd

    def resolve_type(parent, info):
        return parent.libLsofIResult.type

    def resolve_device(parent, info):
        return parent.libLsofIResult.device

    def resolve_node(parent, info):
        return parent.libLsofIResult.node

    def resolve_name(parent, info):
        return parent.libLsofIResult.name

class LsofUaResult(ObjectType):
    command = String()
    pid = Int()
    user = String()
    fd = String()
    type = String()
    device = String()
    name = String()

    def __init__(self, libLsofUaResult):
        self.libLsofUaResult = libLsofUaResult

    def resolve_command(parent, info):
        return parent.libLsofUaResult.command

    def resolve_pid(parent, info):
        return parent.libLsofUaResult.pid

    def resolve_user(parent, info):
        return parent.libLsofUaResult.user

    def resolve_fd(parent, info):
        return parent.libLsofUaResult.fd

    def resolve_type(parent, info):
        return parent.libLsofUaResult.type

    def resolve_device(parent, info):
        return parent.libLsofUaResult.device

    def resolve_name(parent, info):
        return parent.libLsofUaResult.name

class Network(ObjectType):
    network_interfaces = List(NetworkInterface)
    lsof_i = List(LsofIResult)
    lsof_ua = List(LsofUaResult)

    def resolve_network_interfaces(parent, info):
        return list(map(lambda libNetworkInterface: NetworkInterface(libNetworkInterface), libzinlidacpy.get_network_interfaces()))

    def resolve_lsof_i(parent, info):
        return list(map(lambda libLsofIResult: LsofIResult(libLsofIResult), libzinlidacpy.get_lsof__i()))

    def resolve_lsof_ua(parent, info):
        return list(map(lambda libLsofUaResult: LsofUaResult(libLsofUaResult), libzinlidacpy.get_lsof__Ua()))
