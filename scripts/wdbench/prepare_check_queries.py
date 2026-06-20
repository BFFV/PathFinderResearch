import sys

# Prepare results from queries
def prepare_queries(results_a, results_b, query_n, mode='unfixed'):
    chosen_nodes_a = []
    chosen_nodes_b = []

    # Group chosen results
    with open(results_a, 'r') as results_file:
        results = []
        for result in results_file:
            if result[0] == 'Q':  # Only entities
                results.append(result.strip('\n'))
        chosen_nodes_a.append(results[0])
        proportion = 0
        for _ in range(9):
            proportion += 0.1
            chosen_nodes_a.append(results[round(len(results) * proportion) - 1])
        chosen_nodes_a.append(results[-1])
    with open(results_b, 'r') as results_file:
        results = []
        for result in results_file:
            if result[0] == 'Q':  # Only entities
                results.append(result.strip('\n'))
        chosen_nodes_b.append(results[0])
        proportion = 0
        for _ in range(9):
            proportion += 0.1
            chosen_nodes_b.append(results[round(len(results) * proportion) - 1])
        chosen_nodes_b.append(results[-1])

    # Prepare useful information
    with open(f'{query_n}.txt', 'w') as final:
        if mode == 'enum':
            for node in chosen_nodes_a + chosen_nodes_b:
                final.write(f'{node}\n')
        elif mode == 'unfixed':
            for start, end in zip(chosen_nodes_a, chosen_nodes_b):
                final.write(f'{start},{end}\n')

# Group results from queries that can be used later to generate check queries
if __name__ == '__main__':
    try:
        input_file_a = sys.argv[1]
        input_file_b = sys.argv[2]
        query_number = int(sys.argv[3])
        mode = sys.argv[4]
        prepare_queries(input_file_a, input_file_b, query_number, mode=mode)
    except IndexError:
        print('Args are missing!')
