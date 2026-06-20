import sys

# Generate database with 'size' number of diamonds
def generate_files(size):
    with open('nodes.csv', 'w') as node_file, open('edges.csv', 'w') as edge_file:
        node_file.write(f'N0\n')
        for node_id in range(0, size * 3, 3):
            for neighbor in range(1, 4):
                node_file.write(f'N{node_id + neighbor}\n')
            edge_file.write(f'N{node_id},N{node_id + 1}\n')
            edge_file.write(f'N{node_id},N{node_id + 2}\n')
            edge_file.write(f'N{node_id + 1},N{node_id + 3}\n')
            edge_file.write(f'N{node_id + 2},N{node_id + 3}\n')

# Generate CSV files for diamond database
if __name__ == '__main__':
    try:
        diamond_size = int(sys.argv[1])
        if diamond_size < 1:
            print('Diamond size needs to be at least 1!')
        generate_files(diamond_size)
    except IndexError:
        print('Args are missing!')
