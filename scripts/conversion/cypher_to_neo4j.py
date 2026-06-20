import sys

# Convert queries into all semantic modes available in Neo4j
def convert_queries(query_path, out_dir, db_name):
    with open(query_path, 'r') as queries, open(f'{out_dir}/{db_name}_all_trails.txt', 'w') as all_trails, \
        open(f'{out_dir}/{db_name}_any_shortest_walks.txt', 'w') as any_shortest, open(f'{out_dir}/{db_name}_all_shortest_walks.txt', 'w') as all_shortest:
        for query in queries:
            all_trails.write(query)
            any_shortest.write(query.replace(') RETURN', ')) RETURN').replace('p=(', 'p=shortestPath(('))
            all_shortest.write(query.replace(') RETURN', ')) RETURN').replace('p=(', 'p=allShortestPaths(('))

# Convert from Cypher to Neo4j query format
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        output_dir = sys.argv[2]
        database_name = sys.argv[3]
        convert_queries(input_file, output_dir, database_name)
    except IndexError:
        print('Args are missing!')
