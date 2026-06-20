import os
import sys
import time
from http import client as http_client
from subprocess import DEVNULL, Popen, run

"""
Run: python run_mdb.py [DB_NAME] [MODE] [TIMEOUT_MAX] [SEARCH_STRATEGY] [K_VALUE]
    [DB_NAME] Name of the database (wdbench, pokec, diamond, diamond_check)
    [MODE] Semantic mode (
        0: ANY SHORTEST WALKS,
        1: ALL SHORTEST WALKS,
        2: ANY TRAILS (BFS/DFS),
        3: ALL TRAILS (BFS/DFS),
        4: ENDPOINTS,  # Pokec only
        5: SHORTEST K WALKS,
        6: SHORTEST K TRAILS,
        7: SHORTEST K GROUPS WALKS,
        8: SHORTEST K GROUPS TRAILS,
        9: ANY WALKS (DFS),  # WDBench only
        )
    [TIMEOUT_MAX] Max amount of consecutive timeouts:
        (diamond/diamond_check: 1 / 5 (Shortest K) / 40 (Shortest K Groups), pokec: 1, wdbench: inf, endpoints: 1, default: inf)
    [SEARCH_STRATEGY] Search algorithm to use when exploring the graph: bfs/dfs (default: bfs)
    [K_VALUE] K value to use for K semantics (default: 1)
"""

# Relevant paths
ROOT_DIR = os.path.normpath(os.path.join(os.getcwd(), "../.."))
ENGINE_DIR = os.path.join(ROOT_DIR, "mdb")
QUERY_DIR = os.path.join(ROOT_DIR, "queries/mdb")
RESULTS_DIR = os.path.join(ROOT_DIR, "results/mdb")

# Timeout settings
TIMEOUT = 60  # Timeout in seconds
TIMEOUT_COUNT = 0  # Current number of timeouts
TIMEOUT_MAX = float("inf")  # Max amount of consecutive timeouts

# Networking settings
HOST = "127.0.0.1"
PORT = 8080


# Check if server ports are listening
def is_listening(ports):
    status = []
    for port in ports:
        command = run(["sudo", "lsof", "-t", f"-i:{port}"], capture_output=True, text=True)
        listeners = [pid for pid in command.stdout.split("\n") if pid not in ("", str(os.getpid()))]
        status.append(bool(listeners))
    if True not in status:
        return "none"
    if False not in status:
        return "all"
    return "some"


# Start server
def start_server(db_name, path_mode):
    os.chdir(ENGINE_DIR)
    database = f"dbs/{db_name}"
    server = Popen(
        [
            "build/Release/bin/mdb",
            "server",
            database,
            "-t",
            f"{TIMEOUT}",
            "-p",
            f"{PORT}",
            "--path-mode",
            path_mode,
            "--no-browser",
            "--versioned-buffer",
            "64GB",
            "--unversioned-buffer",
            "6GB",
            "--strings-static",
            "9GB",
        ],
        stdin=DEVNULL,
        stdout=DEVNULL,
        stderr=DEVNULL,
    )
    while is_listening([PORT]) != "all":
        time.sleep(1)
    return server


# Close server
def close_server(server):
    server.terminate()
    server.wait()


# Write timeout to results file
def query_timeout(query, query_number, results_file, memory):
    with open(results_file, "a") as out_file:
        out_file.write(f"{query_number},{query.replace(',', '..')},0,TIMEOUT,-1,-1,{memory}\n")


# Execute a single query
def execute_query(query_line, results_file, connection, server_proc):
    split_query = query_line.strip("\n").split(",")
    query_number = split_query[0]
    query = ",".join(split_query[1:])

    # If already timed out multiple consecutive times, assume timeout
    if TIMEOUT_COUNT >= TIMEOUT_MAX:
        mem_command = run(["grep", "^VmRSS", f"/proc/{server_proc.pid}/status"], capture_output=True, text=True)
        memory_mb = round(float(mem_command.stdout.split()[1]) / 1024, 2)  # Get memory in MB
        query_timeout(query, query_number, results_file, memory_mb)
        return True

    # Execute new query
    start = time.time()
    connection.request("POST", "/", headers={"Accept": "text/csv"}, body=query)
    response = connection.getresponse()
    results = response.read().decode("utf-8")
    exec_time = int((time.time() - start) * 1000)

    # Store physical memory usage for the server process
    mem_command = run(["grep", "^VmRSS", f"/proc/{server_proc.pid}/status"], capture_output=True, text=True)
    memory_mb = round(float(mem_command.stdout.split()[1]) / 1024, 2)  # Get memory in MB

    # Store number of results
    n_results = results.count("\n") - 1  # Number of lines in the output, except for the header

    # Save to output file
    enum_time = int((time.time() - start) * 1000)
    if exec_time <= TIMEOUT * 1000:
        with open(results_file, "a") as out_file:
            out_file.write(
                f"{query_number},{query.replace(',', '..')},{n_results},OK,{exec_time},{enum_time},{memory_mb}\n"
            )
        return False
    query_timeout(query, query_number, results_file, memory_mb)  # Timeout
    return True


# Run benchmark for MillenniumDB with a specific mode on a database
if __name__ == "__main__":
    # Parse params
    try:
        database_name = sys.argv[1]
        chosen_mode = int(sys.argv[2])
        if len(sys.argv) >= 4:
            if sys.argv[3] == "inf":
                TIMEOUT_MAX = float("inf")
            else:
                TIMEOUT_MAX = int(sys.argv[3])
        path_mode = "bfs"
        if len(sys.argv) >= 5 and sys.argv[4] == "dfs":
            path_mode = "dfs"
        k_value = 1
        if len(sys.argv) >= 6:
            k_value = int(sys.argv[5])
    except (IndexError, ValueError):
        print("Args are either missing or wrong!")

    # Select mode
    modes = [
        "any_shortest_walks",
        "all_shortest_walks",
        "any_trails",
        "all_trails",
        "endpoints",
        "shortest_k_walks",
        "shortest_k_trails",
        "shortest_k_groups_walks",
        "shortest_k_groups_trails",
        "any_walks",
    ]
    mode = modes[chosen_mode]

    # New K Semantics
    if database_name not in ("diamond", "diamond_check") and chosen_mode in range(5, 9):
        mode = mode.replace("_k_", f"_{str(k_value)}_")

    # Define relevant paths and information
    print(f"Engine: MillenniumDB\nMode: {mode} ({path_mode})\nDatabase: {database_name}\n")
    query_path = os.path.join(QUERY_DIR, f"{database_name}_{mode}.txt")
    results_path = os.path.join(RESULTS_DIR, f"{database_name}_{mode}_{path_mode}.csv")

    # Special Case: Diamond CHECK version (use Diamond database)
    if database_name == "diamond_check":
        database_name = "diamond"

    # Execute benchmark
    print("Loading Database...")
    server_process = start_server(database_name, path_mode)
    print("Starting Benchmark...\n")
    with open(results_path, "w") as out_file:
        out_file.write("query_number,query,results,status,exec_time_ms,enum_time_ms,memory_MB\n")
    with open(query_path, "r") as query_file:
        queries = query_file.readlines()
    conn = http_client.HTTPConnection(f"{HOST}:{PORT}")
    for idx, query_line in enumerate(queries):
        print(f"Processing Query... {idx + 1}/{len(queries)}", end="", flush=True)
        timed_out = execute_query(query_line, results_path, conn, server_process)
        if timed_out:
            print("   TIMEOUT")
            TIMEOUT_COUNT += 1
        else:
            print("   OK")
            TIMEOUT_COUNT = 0
    close_server(server_process)
    print("\nBenchmark Finished!")
