import os
import sys
import time
from neo4j import GraphDatabase, Query
from neo4j.exceptions import TransientError
from subprocess import run

"""
Run: python run_memgraph.py [DB_NAME] [MODE] [TIMEOUT_MAX]
    [DB_NAME] Name of the database
    [MODE] Semantic mode (0: ALL TRAILS, 1: ANY SHORTEST WALKS, 2: ALL SHORTEST WALKS, 3: ENDPOINTS, 4: ALL FIXED)
    [TIMEOUT_MAX] Max amount of consecutive timeouts (diamond: 1, pokec: 11, wdbench: inf, endpoints: 2, default: 1)
"""

# Relevant paths
ROOT_DIR = os.path.normpath(os.path.join(os.getcwd(), '../..'))
ENGINE_DIR = os.path.join(ROOT_DIR, 'memgraph')
QUERY_DIR = os.path.join(ROOT_DIR, 'queries/memgraph')
RESULTS_DIR = os.path.join(ROOT_DIR, 'results/memgraph')

# Timeout settings
TIMEOUT = 60  # Timeout in seconds
TIMEOUT_COUNT = 0  # Current number of timeouts
TIMEOUT_MAX = 1  # Max amount of consecutive timeouts

# Networking settings
IP = '127.0.0.1'
PORT = 7687

# Start client
def start_client():
    client = GraphDatabase.driver(f'bolt://{IP}:{PORT}')
    return client

# Close client
def close_client(client, session):
    session.close()
    client.close()

# Check if server ports are listening
def is_listening(ports):
    status = []
    for port in ports:
        command = run(['sudo', 'lsof', '-t', f'-i:{port}'], capture_output=True, text=True)
        listeners = [pid for pid in command.stdout.split('\n') if pid not in ('', str(os.getpid()))]
        status.append(bool(listeners))
    if True not in status:
        return 'none'
    if False not in status:
        return 'all'
    return 'some'

# Start server
def start_server():
    if is_listening([PORT]) == 'all':  # Bypass server start
        return
    run(['sudo', 'systemctl', 'start', 'memgraph'], capture_output=True)
    while is_listening([PORT]) != 'all':
        time.sleep(1)
    time.sleep(2)

# Close server
def close_server():
    run(['sudo', 'systemctl', 'stop', 'memgraph'], capture_output=True)
    while is_listening([PORT]) != 'none':
        time.sleep(1)

# Write timeout to results file
def query_timeout(query, query_number, results_file):
    with open(results_file, 'a') as out_file:
        out_file.write(f'{query_number},{query[:-1].replace(",", "")},0,TIMEOUT,-1,-1\n')

# Execute a single query
def execute_query(client, query_line, results_file):
    split_query = query_line.strip('\n').split(',')
    query_number = split_query[0]
    query = ','.join(split_query[1:])

    # If already timed out multiple consecutive times, assume timeout
    if TIMEOUT_COUNT >= TIMEOUT_MAX:
        query_timeout(query, query_number, results_file)
        return True

    # Execute new query
    try:
        start = time.time()
        results = client.run(Query(query, timeout=TIMEOUT))
        exec_time = int((time.time() - start) * 1000)
        n_results = 0
        for _ in results:
            n_results += 1
        enum_time = int((time.time() - start) * 1000)
        with open(results_file, 'a') as out_file:
            out_file.write(f'{query_number},{query[:-1].replace(",", "")},{n_results},OK,{exec_time},{enum_time}\n')
        return False
    except TransientError:  # Timeout
        query_timeout(query, query_number, results_file)
        return True

# Run benchmark for Memgraph with a specific mode on a database
if __name__ == '__main__':
    try:
        database_name = sys.argv[1]
        chosen_mode = int(sys.argv[2])
        if len(sys.argv) == 4:
            if sys.argv[3] == 'inf':
                TIMEOUT_MAX = float('inf')
            else:
                TIMEOUT_MAX = int(sys.argv[3])
        modes = ['all_trails', 'any_shortest_walks', 'all_shortest_walks', 'endpoints', 'all_fixed']
        mode = modes[chosen_mode]
        print(f'Engine: Memgraph\nMode: {mode}\nDatabase: {database_name}\n')
        query_path = os.path.join(QUERY_DIR, f'{database_name}_{mode}.txt')
        results_path = os.path.join(RESULTS_DIR, f'{database_name}_{mode}.csv')
        print('Loading Database...')
        server_process = start_server()
        client_process = start_client()
        client_session = client_process.session(database='memgraph')  # Memgraph holds all data in the same space
        print('Starting Benchmark...\n')
        with open(results_path, 'w') as out_file:
            out_file.write('query_number,query,results,status,exec_time_ms,enum_time_ms\n')
        with open(query_path, 'r') as query_file:
            queries = query_file.readlines()
        for idx, query_line in enumerate(queries):
            print(f'Processing Query... {idx + 1}/{len(queries)}', end='', flush=True)
            timed_out = execute_query(client_session, query_line, results_path)
            if timed_out:
                print('   TIMEOUT')
                TIMEOUT_COUNT += 1
            else:
                print('   OK')
                TIMEOUT_COUNT = 0
        close_client(client_process, client_session)
        close_server()
        print('\nBenchmark Finished!')
    except (IndexError, ValueError):
        print('Args are either missing or wrong!')
