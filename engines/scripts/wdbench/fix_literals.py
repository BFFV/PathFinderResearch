import sys

# Remove additional separators from rows
def fix_separators(literals_path):
    with open(literals_path, 'r') as lit_file, open('literals.csv', 'w') as lit_out:
        for lit in lit_file:
            info = lit.strip('\n').split(',')
            if len(info) > 2:  # Extra separators
                lit_out.write(f'{info[0]},{"".join(info[1:])}\n')
            else:
                lit_out.write(lit)

# Fix extra separators present in the literals CSV file
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        fix_separators(input_file)
    except IndexError:
        print('Args are missing!')
