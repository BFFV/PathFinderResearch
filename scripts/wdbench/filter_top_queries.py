import re
import sys

top_props = set(['P279', 'P31', 'P131', 'P171', 'P106', 'P17', 'P40', 'P361', 'P19', 'P39'])

# Filter queries that contain only top props
def filter_queries(query_path):
    with open(query_path, 'r') as queries, open('filtered_queries.txt', 'w') as filtered:
        for query in queries:
            matches = re.findall("[^']P\d+", query)
            prop_matches = [prop[1:] for prop in matches]
            if set(prop_matches).issubset(top_props):
                filtered.write(query)

# Filter relevant queries
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        filter_queries(input_file)
    except IndexError:
        print('Args are missing!')
