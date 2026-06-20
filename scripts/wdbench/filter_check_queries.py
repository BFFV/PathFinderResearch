import sys

# Filter queries without variables
def filter_queries(query_path):
    with open(query_path, 'r') as queries, open('wdbench_check.txt', 'w') as final:
        for query in queries:
            if '?x' not in query:
                final.write(query)

# Filter queries with endpoints fixed
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        filter_queries(input_file)
    except IndexError:
        print('Args are missing!')
