#!/usr/bin/env python3
import json
import os
import subprocess
import sys
import time
from threading import Thread

from SPARQLWrapper import SPARQLWrapper
from SPARQLWrapper.SPARQLExceptions import QueryBadFormed

JSON_CONFIG_PATH: str = "scripts/update-config.json"

with open(JSON_CONFIG_PATH, encoding="utf-8") as json_file:
    config = json.load(json_file)

TESTS_PATH: str = config["tests_directory"]
QUERIES_PATH: str = TESTS_PATH + "/queries"
DB_PATH: str = TESTS_PATH + "/tmp"

IMPORT_CMD = (
    f"build/Release/bin/mdb import {TESTS_PATH}/data.ttl {DB_PATH} --prefixes {TESTS_PATH}/prefixes.txt".split()
)
SERVER_CMD = f"build/Release/bin/mdb server {DB_PATH} --timeout {config['timeout']}".split()


def lsof(pid: int) -> str:
    process = subprocess.Popen(
        ["lsof", "-a", f"-p{pid}", "-i:1234", "-t"], stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    out, _ = process.communicate()
    return out.decode("UTF-8").rstrip()


def create_db() -> None:
    print("Creating database...")
    import_process = subprocess.run(IMPORT_CMD, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if import_process.returncode != 0:
        print(import_process.stdout.decode("utf-8"))
        sys.exit()


def start_server() -> subprocess.Popen[bytes]:
    print("Starting server...")
    server_process = subprocess.Popen(SERVER_CMD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    # Sleep to wait server start
    while not lsof(server_process.pid):
        time.sleep(1)

    return server_process


def kill_server(server_process: subprocess.Popen[bytes]) -> None:
    server_process.kill()
    server_process.wait()

    while lsof(server_process.pid):
        time.sleep(1)


def exec_update(query: str) -> None:
    sparql = SPARQLWrapper("http://localhost:1234/update")

    sparql.setQuery(query)
    sparql.setMethod("POST")
    sparql.setRequestMethod("postdirectly")

    try:
        sparql.query()
    except QueryBadFormed:
        print("QueryBadFormed")


def exec_read_only(query: str, expected: list[str]) -> None:
    sparql = SPARQLWrapper("http://localhost:1234/sparql")

    sparql.setQuery(query)
    sparql.setMethod("POST")
    sparql.setReturnFormat("csv")
    sparql.setRequestMethod("postdirectly")

    try:
        results = sparql.query()
        current = results.response.read().decode("utf-8").strip()
        if current not in expected:
            print("Result is not expected")
            print("Current:")
            print(f"{current}")
            print("Expected:")
            print(f"{expected[0]}\n{expected[1]}")
    except QueryBadFormed:
        print("QueryBadFormed")


def exec_read_only_loop(query: str, expected: list[str]) -> None:
    sparql = SPARQLWrapper("http://localhost:1234/sparql")
    sparql.setQuery(query)
    sparql.setMethod("POST")
    sparql.setReturnFormat("csv")
    sparql.setRequestMethod("postdirectly")

    current = ""

    while current != expected[1]:
        try:
            results = sparql.query()
            current = results.response.read().decode("utf-8").strip()
            if current not in expected:
                print("Result is not expected")
                print("Current:")
                print(f"{current}")
                print("Expected:")
                print(f"{expected[0]}\n{expected[1]}")
                break
        except QueryBadFormed:
            print("QueryBadFormed")
            break


def run_test(test_dir: str) -> None:
    print(f"\nRUNNING TEST: {test_dir}")
    expected: list[str] = []

    with open(f"{QUERIES_PATH}/{test_dir}/before.csv", encoding="utf-8") as file:
        expected.append(file.read().strip())

    with open(f"{QUERIES_PATH}/{test_dir}/after.csv", encoding="utf-8") as file:
        expected.append(file.read().strip())

    with open(f"{QUERIES_PATH}/{test_dir}/update.rq", encoding="utf-8") as file:
        update_query = str(file.read())

    with open(f"{QUERIES_PATH}/{test_dir}/verify.rq", encoding="utf-8") as file:
        verify_query = str(file.read())

    update_thread = Thread(target=exec_update, args=[update_query])
    verify_thread = Thread(target=exec_read_only_loop, args=[verify_query, expected])

    # Query before the update
    exec_read_only(verify_query, expected)

    # Queries during the update
    verify_thread.start()
    update_thread.start()

    update_thread.join()
    verify_thread.join()

    # Query after the update
    exec_read_only(verify_query, expected)


if __name__ == "__main__":
    server_process = start_server()

    for test_dir in os.listdir(QUERIES_PATH):
        run_test(test_dir)

    kill_server(server_process)
