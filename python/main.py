from graphene import Schema
import schema.schema as schema_module

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
        }
    }"""
    print(zinlidac_schema.execute(query))
    