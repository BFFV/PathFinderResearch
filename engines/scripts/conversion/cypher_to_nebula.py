import re
import sys

# Convert queries into all semantic modes available in NebulaGraph
def convert_queries(query_path, out_dir, db_name):
    with open(query_path, 'r') as queries, \
        open(f'{out_dir}/{db_name}_all_trails.txt', 'w') as all_trails, \
        open(f'{out_dir}/{db_name}_any_shortest_trails.txt', 'w') as any_shortest_trails, \
        open(f'{out_dir}/{db_name}_all_shortest_trails.txt', 'w') as all_shortest_trails, \
        open(f'{out_dir}/{db_name}_all_walks.txt', 'w') as all_walks:
        for query in queries:
            # MATCH statement
            all_trails.write(query)
            any_shortest_trails.write(query.replace(') RETURN', ')) RETURN').replace('p=(', 'p=shortestPath(('))
            all_shortest_trails.write(query.replace(') RETURN', ')) RETURN').replace('p=(', 'p=allShortestPaths(('))

            # GO statement
            walks_query = 'GO '
            steps = re.search('\*.+\]', query)
            if steps:
                n_steps = steps[0][1:-1].split('..')
                walks_query += f'{n_steps[0]} TO {n_steps[1]} STEPS '
            else:
                walks_query += '1 TO 2000 STEPS '
            nodes = re.findall('\"[a-zA-Z0-9]+\"', query)
            edge_type = re.search('\[:.+\*', query)[0][2:-1]
            walks_query += f'FROM {nodes[0]} OVER {edge_type} '
            if len(nodes) > 1:
                walks_query += f'WHERE properties($$).id == {nodes[1]} '
            walks_query += 'YIELD src(edge) AS src, dst(edge) AS dst | LIMIT 100000;\n'
            all_walks.write(f'{query.split(",")[0]},{walks_query}')

# Convert from Cypher to NebulaGraph query format
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        output_dir = sys.argv[2]
        database_name = sys.argv[3]
        convert_queries(input_file, output_dir, database_name)
    except IndexError:
        print('Args are missing!')
