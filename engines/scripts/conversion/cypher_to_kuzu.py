import re
import sys

# Convert queries into all semantic modes available in Kuzu
def convert_queries(query_path, out_dir, db_name):
    with open(query_path, 'r') as queries, open(f'{out_dir}/{db_name}_all_walks.txt', 'w') as all_walks, \
        open(f'{out_dir}/{db_name}_any_shortest_walks.txt', 'w') as any_shortest, open(f'{out_dir}/{db_name}_all_shortest_walks.txt', 'w') as all_shortest:
        for query in queries:
            if '..' not in query:
                query = query.replace('*', '*1..30')
            all_walks.write(query)
            if len(re.findall('\[', query)) == 1:
                if '*' in query:
                    any_shortest.write(query.replace('*', '* SHORTEST '))
                    all_shortest.write(query.replace('*', '* ALL SHORTEST '))

# Convert from Cypher to Kuzu query format
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        output_dir = sys.argv[2]
        database_name = sys.argv[3]
        convert_queries(input_file, output_dir, database_name)
    except IndexError:
        print('Args are missing!')
