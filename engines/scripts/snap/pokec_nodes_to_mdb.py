import sys

# Extract nodes and their properties
def process_nodes(data_path, header_path):
    columns = []
    with open(header_path, 'r') as header_file:  # Get header names
        for column in header_file:
            columns.append(column.strip('\n').split())
    with open(data_path, 'r', encoding='utf-8') as data_file, open('mdb_nodes.txt', 'w', encoding='utf-8') as nodes_out:  # Get data
        for node in data_file:
            info = node.strip('\n').split('\t')
            if len(info) != 60:  # Error
                print('Error in format!')
                break
            props_str = f'P{info[0]}'  # Node ID
            for idx, attribute in enumerate(info[1: 59]):  # Node properties
                if attribute != 'null':
                    key, data_type = columns[idx]
                    if data_type == 'int':
                        value = attribute
                    elif data_type == 'bool':
                        value = 'true'
                        if attribute == '0':
                            value = 'false'
                    elif data_type == 'str':
                        clean_text = attribute.replace('\\', '\\\\').replace('\"', '\\"')
                        value = f'"{clean_text}"'
                    props_str += f' {key}:{value}'
            nodes_out.write(f'{props_str}\n')

# Generate a CSV file for nodes, in the MDB format
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        header_file = sys.argv[2]
        process_nodes(input_file, header_file)
    except IndexError:
        print('Args are missing!')
