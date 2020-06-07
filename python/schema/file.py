from graphene import ObjectType, String

import shutil
import os.path

class File(ObjectType):
    src = String()
    dst = String()

    def __init__(self, file_path, base_dir):
        self.file_path = file_path
        self.base_dir = base_dir

    def resolve_src(parent, info):
        return parent.file_path

    def resolve_dst(parent, info):
        file_name = os.path.basename(parent.file_path)
        dst = os.path.join(parent.base_dir, file_name)
        shutil.copyfile(parent.file_path, dst)
        return dst
