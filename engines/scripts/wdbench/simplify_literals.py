import sys

# Remove values from literals
def simplify_literals(literals_path):
    with open(literals_path, 'r') as lit_file, open('literals_simple.csv', 'w') as lit_out:
        for lit in lit_file:
            id_info = lit.strip('\n').split(',')[0]
            lit_out.write(f'{id_info}\n')

# Remove values in the literals CSV file
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        simplify_literals(input_file)
    except IndexError:
        print('Args are missing!')
