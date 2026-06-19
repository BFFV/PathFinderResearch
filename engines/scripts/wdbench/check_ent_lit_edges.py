import sys

# Check IDs for Entities and Literals
def check_ids(ent_path, lit_path, el_path):
    ent_set = set()
    lit_set = set()
    print('Loading ENT')
    with open(ent_path, 'r') as ent_file:
        for line in ent_file:
            info = line.strip('\n').split(',')
            ent_set.add(info[0])
    print('Loading LIT')
    with open(lit_path, 'r') as lit_file:
        for line in lit_file:
            info = line.strip('\n').split(',')
            lit_set.add(info[0])
    print('Checking EDGES ENT-LIT')
    with open(el_path, 'r') as el_file:
        for line in el_file:
            info = line.strip('\n').split(',')
            error = False
            if info[0] not in ent_set:
                error = True
                print(f'NO Entity with ID {info[0]}')
            if info[1] not in lit_set:
                error = True
                print(f'NO Literal with ID {info[1]}')
            if error:
                print(f'Line: {line}')

# Check if ENT-LIT edges are correct
if __name__ == '__main__':
    try:
        entity_file = sys.argv[1]
        literal_file = sys.argv[2]
        el_file = sys.argv[3]
        check_ids(entity_file, literal_file, el_file)
    except IndexError:
        print('Args are missing!')
