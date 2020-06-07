from graphene import ObjectType, String

import subprocess

def execute_command(command):
    try:
        proc = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True, shell=True)
        return proc.stdout.decode('utf-8')
    except subprocess.CalledProcessError as e:
        raise Exception(e.stderr.decode('utf-8'))

class Command(ObjectType):
    command = String()
    result = String()

    def __init__(self, command_str):
        self.command_str = command_str

    def resolve_command(parent, info):
        return parent.command_str

    def resolve_result(parent, info):
        return execute_command(parent.command_str)
