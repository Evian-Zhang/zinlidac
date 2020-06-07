from enum import Enum
import json

from graphene import Schema
from graphql import GraphQLError
from graphql.error import format_error as format_graphql_error
import argparse

import schema.schema as schema_module

class Format(Enum):
    JSON = 1
    ProtoBuf = 2

# see https://github.com/graphql-python/graphene-django/blob/master/graphene_django
def format_error(error):
    if isinstance(error, GraphQLError):
        return format_graphql_error(error)
    else:
        return {'message': str(error)}

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Python wrapper for libzinlidac")
    parser.add_argument("graphql_query", help="path of input GraphQL query file")
    parser.add_argument("-o", "--output", nargs='?', default="results.json", help="path of output results file")

    args = parser.parse_args()

    with open(args.graphql_query, 'r') as query_file:
        query = query_file.read()
        zinlidac_schema = Schema(query=schema_module.Query, auto_camelcase=False)
        query_results = zinlidac_schema.execute(query)
        results = {'data': query_results.data}
        if query_results.errors:
            results['errors'] = [format_error(e) for e in query_results.errors]
        results_str = json.dumps(results, ensure_ascii=False, indent=4)
        with open(args.output, 'w+', encoding="utf-8") as results_file:
            results_file.write(results_str)
