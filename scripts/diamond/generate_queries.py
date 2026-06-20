import sys

# Generate queries of increasing number of paths
def generate_queries(mql=False):
    with open('queries.txt', 'w') as query_out:
        q_id = 1
        if mql:  # Use MQL format
            query_out.write(f'{q_id},MATCH (N2997)=[<SEMANTIC> ?p :E{{1,2}}]=>(N3000) RETURN ?p LIMIT 100000\n')
        else:
            query_out.write(f'{q_id},MATCH p=(:N{{id:"N2997"}})-[:E*1..2]->(:N{{id:"N3000"}}) RETURN p LIMIT 100000;\n')
        q_id += 1
        max_node_id = 3000
        for diamond_size in range(1, 41):
            start_id = max_node_id - 3 * diamond_size
            path_length = 2 * diamond_size
            if mql:  # Use MQL format
                query_out.write(f'{q_id},MATCH (N{start_id})=[<SEMANTIC> ?p :E{{1,{path_length}}}]=>(N3000) RETURN ?p LIMIT 100000\n')
            else:
                query_out.write(f'{q_id},MATCH p=(:N{{id:"N{start_id}"}})-[:E*1..{path_length}]->(:N{{id:"N3000"}}) RETURN p LIMIT 100000;\n')
            q_id += 1

# Generate queries for diamond dataset (size 1000)
if __name__ == '__main__':
    try:
        mdb_format = len(sys.argv) == 2 and sys.argv[1] in ('mdb', 'mql')  # MDB query format
        generate_queries(mql=mdb_format)
    except IndexError:
        print('Args are missing!')
