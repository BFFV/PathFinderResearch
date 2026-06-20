import sys
from collections import Counter

# Generate queries starting from random nodes
def generate_queries(nodes_path, mql=False):
    with open(nodes_path, 'r') as data_file, open('queries.txt', 'w') as query_out:
        # Gather all origin nodes
        edge_nodes = []
        for edge in data_file:
            edge_nodes.append(edge.strip('\n').split(',')[0])

        # Sort nodes and get the sample
        node_freq = Counter(edge_nodes)
        ordered_nodes = [node[0] for node in node_freq.most_common()]
        chosen_nodes = []
        chosen_nodes.append(ordered_nodes[0])  # Most common
        proportion = 0
        for _ in range(9):
            proportion += 0.1
            chosen_nodes.append(ordered_nodes[round(len(ordered_nodes) * proportion) - 1])
        chosen_nodes.append(ordered_nodes[-1])  # Least common
        q_id = 1
        for high in range(1, 31):
            for node in reversed(chosen_nodes):
                if mql:  # Use MQL format
                    if not node[0].isalpha():
                        node = 'P' + node
                    query_out.write(f'{q_id},MATCH ({node})=[<SEMANTIC> ?p :Knows{{1,{high}}}]=>(?n) RETURN ?p LIMIT 100000\n')
                else:
                    query_out.write(f'{q_id},MATCH p=(:Person{{id:"{node}"}})-[:Knows*1..{high}]->(n) RETURN p LIMIT 100000;\n')
                q_id += 1

# Generate queries for SNAP datasets
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        mdb_format = len(sys.argv) == 3 and sys.argv[2] in ('mdb', 'mql')  # MDB query format
        generate_queries(input_file, mql=mdb_format)
    except IndexError:
        print('Args are missing!')
