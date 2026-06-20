import re
import sys

# Convert queries into MQL format
def convert_queries(query_path, limit=100000):
    with open(query_path, 'r') as queries, open('mql_queries.txt', 'w') as mql:
        for query_str in queries:
            query_number, query = query_str.split(',')
            query_parts = query.strip().split(' ')
            from_string = query_parts[0]
            property_path = ' '.join(query_parts[1 : len(query_parts) - 1])
            end_string = query_parts[len(query_parts) - 1]

            # Parse subject
            if from_string[0] == '?':
                from_string = f'({from_string})=[?p '
            else:
                from_string = '(' + from_string.split('/')[-1].replace('>', '') + f')=[?p '

            # Parse object
            if end_string[0] == '?':
                end_string = f']=>({end_string})'
            else:
                end_string = ']=>(' + end_string.split('/')[-1].replace('>', '') + ')'

            # Parse SPARQL property path
            pattern = r'\<[a-zA-Z0-9\/\.\:\#]*\>'
            path_edges = re.findall(pattern, property_path)
            clean_property_path = property_path
            for path in path_edges:
                clean_path = ':' + path.split('/')[-1].replace('>', '')
                clean_property_path = re.sub(path, clean_path, clean_property_path, flags=re.MULTILINE)

            # Write to file
            mql.write(f'{query_number},MATCH {from_string}{clean_property_path}{end_string} RETURN ?p LIMIT {limit}\n')

# Convert from SPARQL to MQL query format
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        if len(sys.argv) == 3:
            convert_queries(input_file, limit=int(sys.argv[2]))
        convert_queries(input_file)
    except IndexError:
        print('Args are missing!')
