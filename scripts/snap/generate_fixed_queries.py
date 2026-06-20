import sys

# Generate queries of increasing path length
def generate_queries(mql=False):
    with open('queries.txt', 'w') as query_out:
        q_id = 1
        for path_length in range(1, 13):
            if mql:  # Use MQL format
                query_out.write(f'{q_id},MATCH (P696585)=[ALL TRAILS ?p :Knows{{{path_length},{path_length}}}]=>(?n) RETURN ?p LIMIT 100000\n')
            else:
                query_out.write(f'{q_id},MATCH p=(:Person{{id:"696585"}})-[:Knows*{path_length}..{path_length}]->(n) RETURN p LIMIT 100000;\n')
            q_id += 1

# Generate queries for SNAP datasets
if __name__ == '__main__':
    try:
        mdb_format = len(sys.argv) == 2 and sys.argv[1] in ('mdb', 'mql')  # MDB query format
        generate_queries(mql=mdb_format)
    except IndexError:
        print('Args are missing!')
