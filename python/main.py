from graphene import Schema
import schema

if __name__ == '__main__':
    schema = Schema(query=schema.Query)