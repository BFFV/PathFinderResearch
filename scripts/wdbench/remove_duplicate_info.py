import sys

# Remove duplicate nodes
def remove_duplicates(result_path, output_path):
    node_set = set()
    with open(result_path, 'r') as results, open(output_path, 'w') as filtered:
        for result_line in results:
            result = result_line.strip('\n')
            if result not in node_set:  # New node
                node_set.add(result)
                filtered.write(result_line)

# Remove duplicate info from result files
if __name__ == '__main__':
    try:
        result_file = sys.argv[1]
        output_file = sys.argv[2]
        remove_duplicates(result_file, output_file)
    except IndexError:
        print('Args are missing!')
