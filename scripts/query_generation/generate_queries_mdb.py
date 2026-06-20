import sys


# Generate queries for Pokec
def generate_queries_pokec(query_dir):
    # Original Semantics
    with open(f'{query_dir}/pokec_any_shortest_walks.txt', 'w') as any_shortest, \
    open(f'{query_dir}/pokec_all_shortest_walks.txt', 'w') as all_shortest, \
    open(f'{query_dir}/pokec_any_trails.txt', 'w') as any_trails, \
    open(f'{query_dir}/pokec_all_trails.txt', 'w') as all_trails, \
    open(f'{query_dir}/pokec_endpoints.txt', 'w') as endpoints:
        q_id = 1
        for path_length in range(1, 13):
            query = f'{q_id},MATCH (P696585)=[<SEMANTIC> ?p :Knows{{1,{path_length}}}]=>(?n) RETURN ?p LIMIT 100000\n'
            any_shortest.write(query.replace('<SEMANTIC>', 'ANY SHORTEST'))
            all_shortest.write(query.replace('<SEMANTIC>', 'ALL SHORTEST'))
            any_trails.write(query.replace('<SEMANTIC>', 'ANY TRAILS'))
            all_trails.write(query.replace('<SEMANTIC>', 'ALL TRAILS'))
            endpoints.write(f'{q_id},MATCH (P696585)=[:Knows{{1,{path_length}}}]=>(?n) RETURN ?n LIMIT 100000\n')

            q_id += 1

    # K Semantics
    with open(f'{query_dir}/pokec_shortest_10_walks.txt', 'w') as shortest_10, \
    open(f'{query_dir}/pokec_shortest_100_walks.txt', 'w') as shortest_100, \
    open(f'{query_dir}/pokec_shortest_1000_walks.txt', 'w') as shortest_1000, \
    open(f'{query_dir}/pokec_shortest_10_trails.txt', 'w') as shortest_10_trails, \
    open(f'{query_dir}/pokec_shortest_100_trails.txt', 'w') as shortest_100_trails, \
    open(f'{query_dir}/pokec_shortest_1000_trails.txt', 'w') as shortest_1000_trails, \
    open(f'{query_dir}/pokec_shortest_1_groups_walks.txt', 'w') as shortest_1_groups, \
    open(f'{query_dir}/pokec_shortest_2_groups_walks.txt', 'w') as shortest_2_groups, \
    open(f'{query_dir}/pokec_shortest_3_groups_walks.txt', 'w') as shortest_3_groups, \
    open(f'{query_dir}/pokec_shortest_4_groups_walks.txt', 'w') as shortest_4_groups, \
    open(f'{query_dir}/pokec_shortest_5_groups_walks.txt', 'w') as shortest_5_groups, \
    open(f'{query_dir}/pokec_shortest_1_groups_trails.txt', 'w') as shortest_1_groups_trails, \
    open(f'{query_dir}/pokec_shortest_2_groups_trails.txt', 'w') as shortest_2_groups_trails, \
    open(f'{query_dir}/pokec_shortest_3_groups_trails.txt', 'w') as shortest_3_groups_trails, \
    open(f'{query_dir}/pokec_shortest_4_groups_trails.txt', 'w') as shortest_4_groups_trails, \
    open(f'{query_dir}/pokec_shortest_5_groups_trails.txt', 'w') as shortest_5_groups_trails:
        q_id = 1
        for path_length in range(1, 13):
            query = f'{q_id},MATCH (P696585)=[<SEMANTIC> ?p :Knows{{1,{path_length}}}]=>(?n) RETURN ?p LIMIT 100000\n'

            # Shortest K Walks/Trails
            shortest_10.write(query.replace('<SEMANTIC>', 'SHORTEST 10 WALKS'))
            shortest_100.write(query.replace('<SEMANTIC>', 'SHORTEST 100 WALKS'))
            shortest_1000.write(query.replace('<SEMANTIC>', 'SHORTEST 1000 WALKS'))
            shortest_10_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 10 TRAILS'))
            shortest_100_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 100 TRAILS'))
            shortest_1000_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 1000 TRAILS'))

            # Shortest K Groups Walks/Trails
            shortest_1_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 1 GROUPS WALKS'))
            shortest_2_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 2 GROUPS WALKS'))
            shortest_3_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 3 GROUPS WALKS'))
            shortest_4_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 4 GROUPS WALKS'))
            shortest_5_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 5 GROUPS WALKS'))
            shortest_1_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 1 GROUPS TRAILS'))
            shortest_2_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 2 GROUPS TRAILS'))
            shortest_3_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 3 GROUPS TRAILS'))
            shortest_4_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 4 GROUPS TRAILS'))
            shortest_5_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 5 GROUPS TRAILS'))

            q_id += 1

# Generate queries for Diamond
def generate_queries_diamond(query_dir):
    with open(f'{query_dir}/diamond_any_shortest_walks.txt', 'w') as any_shortest, \
    open(f'{query_dir}/diamond_all_shortest_walks.txt', 'w') as all_shortest, \
    open(f'{query_dir}/diamond_any_trails.txt', 'w') as any_trails, \
    open(f'{query_dir}/diamond_all_trails.txt', 'w') as all_trails, \
    open(f'{query_dir}/diamond_shortest_k_walks.txt', 'w') as shortest_k, \
    open(f'{query_dir}/diamond_shortest_k_trails.txt', 'w') as shortest_k_trails, \
    open(f'{query_dir}/diamond_shortest_k_groups_walks.txt', 'w') as shortest_k_groups, \
    open(f'{query_dir}/diamond_shortest_k_groups_trails.txt', 'w') as shortest_k_groups_trails:
        # Initial query for cache loading
        q_id = 0
        query = f'{q_id},MATCH (N0)=[ANY SHORTEST ?p :E*]=>(N3000) RETURN ?p LIMIT 100000\n'  # NF = N3000
        for semantic in (any_shortest, all_shortest, any_trails, all_trails, shortest_k, shortest_k_trails,
                         shortest_k_groups, shortest_k_groups_trails):
            semantic.write(query)
        q_id += 1

        # Benchmark queries
        for path_length in range(2, 81, 2):
            query = f'{q_id},MATCH (N0)=[<SEMANTIC> ?p :E{{1,{path_length}}}]=>(?n) RETURN ?p LIMIT 100000\n'
            any_shortest.write(query.replace('<SEMANTIC>', 'ANY SHORTEST'))
            all_shortest.write(query.replace('<SEMANTIC>', 'ALL SHORTEST'))
            any_trails.write(query.replace('<SEMANTIC>', 'ANY TRAILS'))
            all_trails.write(query.replace('<SEMANTIC>', 'ALL TRAILS'))

            # Shortest K Walks/Trails
            k_values = [10, 100, 1000, 10000, 100000]
            for idx, k in enumerate(k_values):
                new_q_id = (q_id - 1) * len(k_values) + (idx + 1)
                shortest_k.write(query.replace('<SEMANTIC>', f'SHORTEST {k} WALKS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))
                shortest_k_trails.write(query.replace('<SEMANTIC>', f'SHORTEST {k} TRAILS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))

            # Shortest K Groups Walks/Trails
            k_values = list(range(1, 41))
            for idx, k in enumerate(k_values):
                new_q_id = (q_id - 1) * len(k_values) + (idx + 1)
                shortest_k_groups.write(query.replace('<SEMANTIC>', f'SHORTEST {k} GROUPS WALKS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))
                shortest_k_groups_trails.write(query.replace('<SEMANTIC>', f'SHORTEST {k} GROUPS TRAILS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))

            q_id += 1

# Generate queries for Diamond (CHECK version)
def generate_queries_diamond_check(query_dir):
    with open(f'{query_dir}/diamond_check_any_shortest_walks.txt', 'w') as any_shortest, \
    open(f'{query_dir}/diamond_check_all_shortest_walks.txt', 'w') as all_shortest, \
    open(f'{query_dir}/diamond_check_any_trails.txt', 'w') as any_trails, \
    open(f'{query_dir}/diamond_check_all_trails.txt', 'w') as all_trails, \
    open(f'{query_dir}/diamond_check_shortest_k_walks.txt', 'w') as shortest_k, \
    open(f'{query_dir}/diamond_check_shortest_k_trails.txt', 'w') as shortest_k_trails, \
    open(f'{query_dir}/diamond_check_shortest_k_groups_walks.txt', 'w') as shortest_k_groups, \
    open(f'{query_dir}/diamond_check_shortest_k_groups_trails.txt', 'w') as shortest_k_groups_trails:
        # Initial query for cache loading
        q_id = 0
        query = f'{q_id},MATCH (N0)=[ANY SHORTEST ?p :E*]=>(N3000) RETURN ?p LIMIT 100000\n'  # NF = N3000
        for semantic in (any_shortest, all_shortest, any_trails, all_trails, shortest_k, shortest_k_trails,
                         shortest_k_groups, shortest_k_groups_trails):
            semantic.write(query)
        q_id += 1

        # Benchmark queries
        for path_length in range(2, 81, 2):
            dest_node_id = (path_length // 2) * 3
            query = f'{q_id},MATCH (N0)=[<SEMANTIC> ?p :E{{1,{path_length}}}]=>(N{dest_node_id}) RETURN ?p LIMIT 100000\n'
            any_shortest.write(query.replace('<SEMANTIC>', 'ANY SHORTEST'))
            all_shortest.write(query.replace('<SEMANTIC>', 'ALL SHORTEST'))
            any_trails.write(query.replace('<SEMANTIC>', 'ANY TRAILS'))
            all_trails.write(query.replace('<SEMANTIC>', 'ALL TRAILS'))

            # Shortest K Walks/Trails
            k_values = [10, 100, 1000, 10000, 100000]
            for idx, k in enumerate(k_values):
                new_q_id = (q_id - 1) * len(k_values) + (idx + 1)
                shortest_k.write(query.replace('<SEMANTIC>', f'SHORTEST {k} WALKS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))
                shortest_k_trails.write(query.replace('<SEMANTIC>', f'SHORTEST {k} TRAILS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))

            # Shortest K Groups Walks/Trails
            k_values = list(range(1, 41))
            for idx, k in enumerate(k_values):
                new_q_id = (q_id - 1) * len(k_values) + (idx + 1)
                shortest_k_groups.write(query.replace('<SEMANTIC>', f'SHORTEST {k} GROUPS WALKS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))
                shortest_k_groups_trails.write(query.replace('<SEMANTIC>', f'SHORTEST {k} GROUPS TRAILS').replace(f'{q_id},MATCH', f'{new_q_id},MATCH'))

            q_id += 1

# Generate queries for WDBench
def generate_queries_wdbench(query_dir):
    # Original Semantics
    with open(f'{query_dir}/wdbench_any_shortest_walks.txt', 'w') as any_shortest, \
    open(f'{query_dir}/wdbench_all_shortest_walks.txt', 'w') as all_shortest, \
    open(f'{query_dir}/wdbench_any_trails.txt', 'w') as any_trails, \
    open(f'{query_dir}/wdbench_all_trails.txt', 'w') as all_trails, \
    open(f'{query_dir}/wdbench_any_walks.txt', 'w') as any_walks, \
    open('wdbench_queries.txt', 'r') as original_queries:
        q_id = 1
        for wdbench_query in original_queries:
            query = f'{q_id},' + wdbench_query.replace('[?p', '[<SEMANTIC> ?p')[wdbench_query.find('MATCH'):].strip('\n') + '\n'
            any_shortest.write(query.replace('<SEMANTIC>', 'ANY SHORTEST'))
            all_shortest.write(query.replace('<SEMANTIC>', 'ALL SHORTEST'))
            any_trails.write(query.replace('<SEMANTIC>', 'ANY TRAILS'))
            all_trails.write(query.replace('<SEMANTIC>', 'ALL TRAILS'))
            any_walks.write(query.replace('<SEMANTIC>', 'ANY'))

            q_id += 1

    # K Semantics
    with open(f'{query_dir}/wdbench_shortest_10_walks.txt', 'w') as shortest_10, \
    open(f'{query_dir}/wdbench_shortest_100_walks.txt', 'w') as shortest_100, \
    open(f'{query_dir}/wdbench_shortest_1000_walks.txt', 'w') as shortest_1000, \
    open(f'{query_dir}/wdbench_shortest_10_trails.txt', 'w') as shortest_10_trails, \
    open(f'{query_dir}/wdbench_shortest_100_trails.txt', 'w') as shortest_100_trails, \
    open(f'{query_dir}/wdbench_shortest_1000_trails.txt', 'w') as shortest_1000_trails, \
    open(f'{query_dir}/wdbench_shortest_1_groups_walks.txt', 'w') as shortest_1_groups, \
    open(f'{query_dir}/wdbench_shortest_2_groups_walks.txt', 'w') as shortest_2_groups, \
    open(f'{query_dir}/wdbench_shortest_3_groups_walks.txt', 'w') as shortest_3_groups, \
    open(f'{query_dir}/wdbench_shortest_4_groups_walks.txt', 'w') as shortest_4_groups, \
    open(f'{query_dir}/wdbench_shortest_5_groups_walks.txt', 'w') as shortest_5_groups, \
    open(f'{query_dir}/wdbench_shortest_1_groups_trails.txt', 'w') as shortest_1_groups_trails, \
    open(f'{query_dir}/wdbench_shortest_2_groups_trails.txt', 'w') as shortest_2_groups_trails, \
    open(f'{query_dir}/wdbench_shortest_3_groups_trails.txt', 'w') as shortest_3_groups_trails, \
    open(f'{query_dir}/wdbench_shortest_4_groups_trails.txt', 'w') as shortest_4_groups_trails, \
    open(f'{query_dir}/wdbench_shortest_5_groups_trails.txt', 'w') as shortest_5_groups_trails, \
    open('wdbench_queries.txt', 'r') as original_queries:
        q_id = 1
        for wdbench_query in original_queries:
            query = f'{q_id},' + wdbench_query.replace('[?p', '[<SEMANTIC> ?p')[wdbench_query.find('MATCH'):].strip('\n') + '\n'

            # Skip queries with both ends unfixed for the Shortest K semantics
            if query.count('(?x') >= 2:
                q_id += 1
                continue

            # Shortest K Walks/Trails
            shortest_10.write(query.replace('<SEMANTIC>', 'SHORTEST 10 WALKS'))
            shortest_100.write(query.replace('<SEMANTIC>', 'SHORTEST 100 WALKS'))
            shortest_1000.write(query.replace('<SEMANTIC>', 'SHORTEST 1000 WALKS'))
            shortest_10_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 10 TRAILS'))
            shortest_100_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 100 TRAILS'))
            shortest_1000_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 1000 TRAILS'))

            # Shortest K Groups Walks/Trails
            shortest_1_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 1 GROUPS WALKS'))
            shortest_2_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 2 GROUPS WALKS'))
            shortest_3_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 3 GROUPS WALKS'))
            shortest_4_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 4 GROUPS WALKS'))
            shortest_5_groups.write(query.replace('<SEMANTIC>', 'SHORTEST 5 GROUPS WALKS'))
            shortest_1_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 1 GROUPS TRAILS'))
            shortest_2_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 2 GROUPS TRAILS'))
            shortest_3_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 3 GROUPS TRAILS'))
            shortest_4_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 4 GROUPS TRAILS'))
            shortest_5_groups_trails.write(query.replace('<SEMANTIC>', 'SHORTEST 5 GROUPS TRAILS'))

            q_id += 1

# Generate queries for benchmarking different databases with MDB
if __name__ == '__main__':
    try:
        query_dir = sys.argv[1]
        generate_queries_pokec(query_dir)
        generate_queries_diamond(query_dir)
        generate_queries_diamond_check(query_dir)
        generate_queries_wdbench(query_dir)
    except IndexError:
        print('Args for query directory are missing!')
