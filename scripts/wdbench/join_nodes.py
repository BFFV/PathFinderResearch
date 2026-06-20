import sys

# Join Entities and Literals in a single file, without the values from Literals
def join_nodes(ent_path, lit_path):
    with open(ent_path, 'r') as ent_file, open(lit_path, 'r') as lit_file, open('nodes.csv', 'w') as nodes_out:
        for line in ent_file:
            ent_id = line.strip('\n')
            nodes_out.write(f'{ent_id}\n')
        for line in lit_file:
            lit_id = line.strip('\n')
            nodes_out.write(f'{lit_id}\n')

# Join Entities and Literals
if __name__ == '__main__':
    try:
        entity_file = sys.argv[1]
        literal_file = sys.argv[2]
        join_nodes(entity_file, literal_file)
    except IndexError:
        print('Args are missing!')
