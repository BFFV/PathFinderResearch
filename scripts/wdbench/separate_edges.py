import sys

# Separate edge data into multiple CSV files
def separate_edges(edges_path):
    with open(edges_path, 'r') as edge_file, open('entities_entities.csv', 'w') as ent_out, open('entities_literals.csv', 'w') as lit_out:
        for edge in edge_file:
            info = edge.strip('\n').split(',')
            if info[1][0].isnumeric():  # Literal
                lit_out.write(edge)
            else:  # Entity
                ent_out.write(edge)

# Generate separate CSV files for Neo4j edges
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        separate_edges(input_file)
    except IndexError:
        print('Args are missing!')
