import sys

top_props = ['P279', 'P31', 'P131', 'P171', 'P106', 'P17', 'P40', 'P361', 'P19', 'P39']

# Separate edge data into multiple CSV files for the most common properties
def separate_props(edges_path, output_dir_path):
    prop_dict = {prop: open(f'{output_dir_path}/{prop}.csv', 'w') for prop in top_props}  # Create all necessary files
    extra_properties_file = open(f'{output_dir_path}/PX.csv', 'w')  # File for all other edges
    with open(edges_path, 'r') as edge_file:
        for edge in edge_file:
            info = edge.strip('\n').split(',')
            if info[2] in top_props:  # Top ranked property
                prop_dict[info[2]].write(f'{info[0]},{info[1]}\n')
            else:  # Other properties
                extra_properties_file.write(f'{info[0]},{info[1]}\n')
    for prop in prop_dict:  # Close all files
        prop_dict[prop].close()
    extra_properties_file.close()

# Generate separate CSV files for relevant edges
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        output_dir = sys.argv[2]
        separate_props(input_file, output_dir)
    except IndexError:
        print('Args are missing!')
