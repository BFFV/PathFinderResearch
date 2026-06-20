import sys

# Generate queries of increasing number of endpoints
def generate_queries(mql=False):
    with open('queries.txt', 'w') as query_out:
        q_id = 1
        if mql:  # Use MQL format
            query_out.write(f'{q_id},MATCH (P696585)=[:Knows*]=>(?n) RETURN ?n LIMIT 100000\n')
        else:
            query_out.write(f'{q_id},MATCH (:Person{{id:"696585"}})-[:Knows*]->(n) RETURN n LIMIT 100000;\n')
        q_id += 1
        for path_length in range(1, 13):
            if mql:  # Use MQL format
                query_out.write(f'{q_id},MATCH (P696585)=[:Knows{{1,{path_length}}}]=>(?n) RETURN ?n LIMIT 100000\n')
            else:
                query_out.write(f'{q_id},MATCH (:Person{{id:"696585"}})-[:Knows*1..{path_length}]->(n) RETURN n LIMIT 100000;\n')
            q_id += 1

# Generate queries of increasing number of endpoints for SPARQL
def generate_sparql_queries():
    with open('queries.txt', 'w') as query_out:
        q_id = 1
        query_out.write(f'{q_id},PREFIX : <http://p.db/> SELECT * WHERE {{:696585 :Knows* ?x}} LIMIT 100000\n')
        q_id += 1
        for path_length in range(1, 13):
            path_exp = ':Knows'
            for _ in range(1, path_length):
                path_exp += '/:Knows?'
            query_out.write(f'{q_id},PREFIX : <http://p.db/> SELECT * WHERE {{:696585 {path_exp} ?x}} LIMIT 100000\n')
            q_id += 1

# Generate queries for SNAP datasets
if __name__ == '__main__':
    try:
        mode = sys.argv[1]  # gql, sparql, mql/mdb
        if mode == 'sparql':
            generate_sparql_queries()
        else:
            mdb_format = mode in ('mdb', 'mql')  # MDB query format
            generate_queries(mql=mdb_format)
    except IndexError:
        print('Args are missing!')
