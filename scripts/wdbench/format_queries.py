import re
import sys

# Format queries by adapting operators to Kuzu
def format_queries(query_path):
    with open(query_path, 'r') as queries, open('wdbench_cypher_final.txt', 'w') as final:
        for query in queries:
            path_query = query.replace('RETURN *', 'RETURN p LIMIT 100000;').replace('MATCH (', 'MATCH p=(')
            path_query = re.sub('\*...]', '*1..30]', path_query)
            path_query = re.sub('\*....]', '*1..1]', path_query)
            path_query = path_query.replace('(0)', '(varx)')
            if '(:' in path_query:
                final.write(path_query)

# Format queries for Kuzu
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        format_queries(input_file)
    except IndexError:
        print('Args are missing!')
