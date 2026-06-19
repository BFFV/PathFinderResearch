import sys

# Convert queries into all semantic modes available in MDB
def convert_queries(query_path, out_dir, db_name):
    with open(query_path, 'r') as queries, open(f'{out_dir}/{db_name}_all_trails.txt', 'w') as all_trails, \
        open(f'{out_dir}/{db_name}_any_trails.txt', 'w') as any_trails, \
        open(f'{out_dir}/{db_name}_any_shortest_walks.txt', 'w') as any_shortest, open(f'{out_dir}/{db_name}_all_shortest_walks.txt', 'w') as all_shortest:
        for query in queries:
            all_trails.write(query.replace('<SEMANTIC>', 'ALL TRAILS'))
            any_trails.write(query.replace('<SEMANTIC>', 'ANY TRAILS'))
            any_shortest.write(query.replace('<SEMANTIC>', 'ANY SHORTEST'))
            all_shortest.write(query.replace('<SEMANTIC>', 'ALL SHORTEST'))

# Convert generic MQL queries into their real semantics
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        output_dir = sys.argv[2]
        database_name = sys.argv[3]
        convert_queries(input_file, output_dir, database_name)
    except IndexError:
        print('Args are missing!')
