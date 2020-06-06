from graphene import ObjectType, String, Int, Field, List

import libzinlidacpy

class FilesystemInfo(ObjectType):
    name = String()
    dir = String()
    mount_type = String()
    mount_options = String()

    def __init__(self, libFilesystemInfo):
        self.libFilesystemInfo = libFilesystemInfo

    def resolve_name(parent, info):
        return parent.libFilesystemInfo.name

    def resolve_dir(parent, info):
        return parent.libFilesystemInfo.dir

    def resolve_mount_type(parent, info):
        return parent.libFilesystemInfo.mount_type

    def resolve_mount_options(parent, info):
        return parent.libFilesystemInfo.mount_options

class Filesystem(ObjectType):
    filesystem_capacity = Int()
    filesystem_free_size = Int()
    filesystem_type = String()
    current_mounted_filesystems = List(FilesystemInfo)
    configured_mounted_filesystems = List(FilesystemInfo)

    def resolve_filesystem_capacity(parent, info):
        return libzinlidacpy.get_filesystem_capacity()

    def resolve_filesystem_free_size(parent, info):
        return libzinlidacpy.get_filesystem_free_size()

    def resolve_filesystem_type(parent, info):
        return libzinlidacpy.get_filesystem_type()

    def resolve_current_mounted_filesystems(parent, info):
        return list(map(lambda libFilesystemInfo: FilesystemInfo(libFilesystemInfo), libzinlidacpy.get_current_mounted_filesystems()))

    def resolve_configured_mounted_filesystems(parent, info):
        return list(map(lambda libFilesystemInfo: FilesystemInfo(libFilesystemInfo), libzinlidacpy.get_configured_mounted_filesystems()))
