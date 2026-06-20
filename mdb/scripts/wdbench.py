from __future__ import annotations

import multiprocessing
import os
import subprocess
import sys
import time
from subprocess import Popen
from typing import Any, Dict

from SPARQLWrapper import JSON, SPARQLWrapper

# import re
# import traceback
# from socket import timeout

###################### EDIT THIS PARAMETERS ######################
TIMEOUT = 600  # Max time per query in seconds
MDB_ROOT = "/data2/benchmark/MillenniumDB"

SERVER_CMD = (
    f"build/Release/bin/mdb server data/wikidata-wcg-lf --shared-buffer 64GB --threads 1 --timeout {TIMEOUT}".split()
)

PORT = 8080
ENDPOINT = "http://localhost:8080/sparql"
##################################################################


def lsof(pid: int) -> str:
    process = subprocess.Popen(
        ["lsof", "-a", f"-p{pid}", f"-i:{PORT}", "-t"], stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    out, _ = process.communicate()
    return out.decode("UTF-8").rstrip()


def lsof_any() -> str:
    process = subprocess.Popen(["lsof", "-t", f"-i:{PORT}"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, _ = process.communicate()
    return out.decode("UTF-8").rstrip()


def start_server() -> Popen[bytes]:
    print("starting server...")

    server_log.write("[start server]\n")
    server_process = subprocess.Popen(SERVER_CMD, stdout=server_log, stderr=server_log)

    # Sleep to wait server start
    while not lsof(server_process.pid):
        time.sleep(1)

    print("done")
    return server_process


def kill_server(server_process: Popen[bytes]) -> None:
    print(f"killing server[{server_process.pid}]...")
    server_log.write("[kill server]\n")

    server_process.kill()
    server_process.wait()

    while lsof(server_process.pid):
        time.sleep(1)

    print("done")


def parse_to_sparql(query: str) -> str:
    if LIMIT == 0:
        return f"SELECT * WHERE {{ {query} }}"
    return f"SELECT * WHERE {{ {query} }} LIMIT {LIMIT}"


# Send query to server
def execute_queries(server_process: Popen[bytes]) -> Popen[bytes]:
    with open(QUERIES_FILE, encoding="utf-8") as queries_file:
        query_number = 1
        for line in queries_file:
            query = line.strip()
            print(f"Executing query {query_number}")
            server_process = query_sparql(server_process, query, query_number)
            query_number += 1
    return server_process


def execute_sparql_wrapper(query_pattern: str, query_number: str) -> None:
    query = parse_to_sparql(query_pattern)

    sparql_wrapper = SPARQLWrapper(ENDPOINT)
    # sparql_wrapper.setTimeout(TIMEOUT+10) # Give 10 more seconds for a chance to graceful timeout
    sparql_wrapper.setReturnFormat(JSON)
    sparql_wrapper.setQuery(query)

    count = 0
    start_time = time.perf_counter_ns()

    try:
        # Compute query
        response = sparql_wrapper.query()
        json_results: Dict[Any, Any] = response.convert()  # type: ignore
        elapsed_time = time.perf_counter_ns() - start_time  # nanoseconds
        for _ in json_results["results"]["bindings"]:
            count += 1

        print(count)

        with open(RESUME_FILE, "a", encoding="utf-8") as file:
            file.write(f"{query_number},{count},OK,{elapsed_time}\n")

    except Exception as e:
        elapsed_time = time.perf_counter_ns() - start_time  # nanoseconds
        with open(RESUME_FILE, "a", encoding="utf-8") as file:
            file.write(f"{query_number},,ERROR({type(e).__name__}),{elapsed_time}\n")

        with open(ERROR_FILE, "a", encoding="utf-8") as file:
            file.write(f"Exception in query {str(query_number)} [{type(e).__name__}]: {str(e)}\n")


def query_sparql(server_process: Popen[bytes], query_pattern: str, query_number: str) -> Popen[bytes]:
    start_time = time.perf_counter_ns()

    try:
        p = multiprocessing.Process(target=execute_sparql_wrapper, args=[query_pattern, query_number])
        p.start()
        # Give 2 more seconds for a chance to graceful timeout or enumerate the results
        p.join(TIMEOUT + 2)
        if p.is_alive():
            p.kill()
            p.join()
            raise Exception("PROCESS_TIMEOUT")
        return server_process

    except Exception as e:
        elapsed_time = time.perf_counter_ns() - start_time  # nanoseconds
        with open(RESUME_FILE, "a", encoding="utf-8") as file:
            file.write(f"{query_number},,PROCESS TIMEOUT({type(e).__name__}),{elapsed_time}\n")

        with open(ERROR_FILE, "a", encoding="utf-8") as file:
            file.write(f"Exception in query {query_number} [{type(e).__name__}]: {str(e)}\n")

        kill_server(server_process)
        return start_server()


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("usage:")
        print(f"python3 {os.path.basename(__file__)} <QUERIES_FILE> <LIMIT> <PREFIX_NAME>")
        print("LIMIT = 0 will not add a limit")
        sys.exit(1)

    # Db engine that will execute queries
    QUERIES_FILE = os.path.abspath(sys.argv[1])
    LIMIT = int(sys.argv[2])
    PREFIX_NAME = sys.argv[3]

    # Path to needed output and input files
    RESUME_FILE = f"{MDB_ROOT}/out/{PREFIX_NAME}_limit{LIMIT}.csv"
    ERROR_FILE = f"{MDB_ROOT}/out/errors/{PREFIX_NAME}_limit{LIMIT}.log"
    SERVER_LOG_FILE = f"{MDB_ROOT}/out/log/{PREFIX_NAME}_limit{LIMIT}.log"

    if lsof_any():
        raise Exception(f"other server already running in port {PORT}")

    # Check if output file already exists
    if os.path.exists(RESUME_FILE):
        print(f"File {RESUME_FILE} already exists.")
        sys.exit()

    server_log = open(SERVER_LOG_FILE, "w", encoding="utf-8")
    os.chdir(MDB_ROOT)

    with open(RESUME_FILE, "w", encoding="utf-8") as file:
        file.write("query_number,results,status,time\n")

    with open(ERROR_FILE, "w", encoding="utf-8") as file:
        file.write("")  # to replace the old error file

    print("benchmark is starting. TIMEOUT:", TIMEOUT, "seconds")
    server_process = start_server()
    server_process = execute_queries(server_process)
    kill_server(server_process)

    server_log.close()
