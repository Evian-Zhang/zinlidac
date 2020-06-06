from graphene import Schema

import schema.schema as schema_module
import libzinlidacpy

if __name__ == '__main__':
    zinlidac_schema = Schema(query=schema_module.Query, auto_camelcase=False)
    query = """{ 
        hardware { 
            cpu { hardware_type } 
            cache { level4_cache_size }
            memory { available_physical_memory_pages_number }
        }
        system {
            operating_system { system_name }
            filesystem {
                filesystem_capacity
                current_mounted_filesystems {
                    name
                    dir
                    mount_type
                    mount_options
                }
            }
            user {
                associated_user {
                    name
                    uid
                }
                users {
                    name
                    login_shell
                }
                groups {
                    name
                }
            }
            process {
                processes {
                    cmdline_arguments,
                    basename
                }
            }
            network {
                network_interfaces {
                    name
                    address
                }
                lsof_i {
                    command
                    user
                    fd
                }
            }
        }
    }"""
    print(zinlidac_schema.execute(query))
    