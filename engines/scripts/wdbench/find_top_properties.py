import re
import sys
from collections import Counter

# Search for the most common graph properties in a query file
def search_properties(query_path):
    with open(query_path, 'r') as queries:
        matches = re.findall("[^']P\d+", queries.read())
        prop_matches = [prop[1:] for prop in matches]
        top_props = Counter(prop_matches).most_common(10)
        print([top_prop[0] for top_prop in top_props])
        print_for_kuzu(top_props)
        print_for_nebula(top_props)

# Print import lines for Kuzu edges
def print_for_kuzu(top):
    print('***KUZU***')
    for top_prop in top:
        print(f'CREATE REL TABLE {top_prop[0]}(FROM Entity TO Entity);')
        print(f'COPY {top_prop[0]} FROM "../dbs/{top_prop[0]}.csv";')
    print(f'CREATE REL TABLE PX(FROM Entity TO Entity);')
    print(f'COPY PX FROM "../dbs/PX.csv";')
    print()

# Print import lines for NebulaGraph edges
def print_for_nebula(top):
    print('***NEBULA***')
    for top_prop in top:
        print(f'CREATE EDGE {top_prop[0]}();')
    for top_prop in top:
        print(f'  - path: ../dbs/{top_prop[0]}.csv\n    edges:\n    - name: {top_prop[0]}')
        print(f'      src:\n        id:\n          type: "STRING"\n          index: 0\n      dst:\n        id:\n          type: "STRING"\n          index: 1')
    print(f'CREATE EDGE PX();')
    print(f'  - path: ../dbs/PX.csv\n    edges:\n    - name: PX')
    print(f'      src:\n        id:\n          type: "STRING"\n          index: 0\n      dst:\n        id:\n          type: "STRING"\n          index: 1')
    print()

# Find most common properties in query set
if __name__ == '__main__':
    try:
        query_file = sys.argv[1]
        search_properties(query_file)
    except IndexError:
        print('Args are missing!')
