import sys

top_queries = ['118', '123', '201', '208', '212', '213', '645', '655', '307', '419', '117', '122', '134', '115', '200']

# Construct new check queries based on original queries and their results
def generate_queries(query_path, nodes_dir_path):
    with open(query_path, 'r') as queries, open('check_queries.txt', 'w') as new_queries:
        q_id = 1
        for query_line in queries:
            query_n, query = query_line.strip('\n').split(',')
            if query_n in top_queries:  # Top hardest query
                query_vars = query.count('?x')
                with open(f'{nodes_dir_path}/{query_n}.txt', 'r') as nodes:
                    if query_vars == 1:  # Enum query
                        for node_line in nodes:
                            node = f'<http://www.wikidata.org/entity/{node_line.strip()}>'
                            start, pattern, end = query.split(' ')
                            if start[0] == '?':  # Start is the var
                                new_queries.write(f'{q_id},{node} {pattern} {end}\n')
                            else:  # End is the var
                                new_queries.write(f'{q_id},{start} {pattern} {node}\n')
                            q_id += 1
                    elif query_vars == 2:  # Unfixed query
                        for node_pair in nodes:
                            start_node, end_node = node_pair.strip('\n').split(',')
                            start = f'<http://www.wikidata.org/entity/{start_node}>'
                            end = f'<http://www.wikidata.org/entity/{end_node}>'
                            _, pattern, _ = query.split(' ')
                            new_queries.write(f'{q_id},{start} {pattern} {end}\n')
                            q_id += 1
            elif '?x' not in query:  # Check query
                new_queries.write(f'{q_id},{query}\n')
                q_id += 1

# Generate check type queries based on results from enum/unfixed queries
if __name__ == '__main__':
    try:
        query_file = sys.argv[1]
        nodes_dir = sys.argv[2]
        generate_queries(query_file, nodes_dir)
    except IndexError:
        print('Args are missing!')
