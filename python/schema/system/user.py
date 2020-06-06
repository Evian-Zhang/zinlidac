from graphene import ObjectType, String, Int, Field, List

import libzinlidacpy

class LoginUserInfo(ObjectType):
    name = String()
    uid = Int()

    def __init__(self, libLoginUserInfo):
        self.libLoginUserInfo = libLoginUserInfo

    def resolve_name(parent, info):
        return parent.libLoginUserInfo.name
    
    def resolve_uid(parent, info):
        return parent.libLoginUserInfo.uid

class UserInfo(ObjectType):
    name = String()
    uid = Int()
    groupid = Int()
    full_name = String()
    home_directory = String()
    login_shell = String()

    def __init__(self, libUserInfo):
        self.libUserInfo = libUserInfo

    def resolve_name(parent, info):
        return parent.libUserInfo.name

    def resolve_uid(parent, info):
        return parent.libUserInfo.uid

    def resolve_groupid(parent, info):
        return parent.libUserInfo.groupid

    def resolve_full_name(parent, info):
        return parent.libUserInfo.full_name

    def resolve_home_directory(parent, info):
        return parent.libUserInfo.home_directory

    def resolve_login_shell(parent, info):
        return parent.libUserInfo.login_shell

class GroupInfo(ObjectType):
    name = String()
    gid = Int()
    users = List(String)

    def __init__(self, libGroupInfo):
        self.libGroupInfo = libGroupInfo

    def resolve_name(parent, info):
        return parent.libGroupInfo.name

    def resolve_gid(parent, info):
        return parent.libGroupInfo.gid

    def resolve_users(parent, info):
        return parent.libGroupInfo.users

class User(ObjectType):
    associated_user = Field(LoginUserInfo)
    users = List(UserInfo)
    groups = List(GroupInfo)

    def resolve_associated_user(parent, info):
        return LoginUserInfo(libzinlidacpy.get_associated_user())

    def resolve_users(parent, info):
        return list(map(lambda libUserInfo: UserInfo(libUserInfo), libzinlidacpy.get_users()))

    def resolve_groups(parent, info):
        return list(map(lambda libGroupInfo: GroupInfo(libGroupInfo), libzinlidacpy.get_groups()))
