#!/usr/bin/env python3

import json
import sys
import time
from threading import Thread

from SPARQLWrapper import SPARQLWrapper
from SPARQLWrapper.SPARQLExceptions import QueryBadFormed

query = """
SELECT (COUNT(*) as ?c)
WHERE {
    ?v0 <http://www.wikidata.org/prop/direct/P166> ?v1 .
    ?v0 <http://www.wikidata.org/prop/direct/P21> <http://www.wikidata.org/entity/Q6581072> .
    ?v2 <http://www.wikidata.org/prop/direct/P166> ?v1 .
    ?v2 <http://www.wikidata.org/prop/direct/P21> <http://www.wikidata.org/entity/Q6581097>
}
LIMIT 100000
"""


def get_insert(i: int):
    return (
        "INSERT DATA {"
        + f"<http://www.ex.com/{i}>   <http://www.wikidata.org/prop/direct/P166> <http://www.ex.com/{i+1}> . "
        + f"<http://www.ex.com/{i}>   <http://www.wikidata.org/prop/direct/P21>  <http://www.wikidata.org/entity/Q6581072> . "
        + f"<http://www.ex.com/{i+2}> <http://www.wikidata.org/prop/direct/P166> <http://www.ex.com/{i+1}> . "
        + f"<http://www.ex.com/{i+2}> <http://www.wikidata.org/prop/direct/P21>  <http://www.wikidata.org/entity/Q6581097>"
        + "}"
    )


def get_delete(i: int):
    return (
        "DELETE DATA {"
        + f"<http://www.ex.com/{i}>   <http://www.wikidata.org/prop/direct/P166> <http://www.ex.com/{i+1}> . "
        + f"<http://www.ex.com/{i}>   <http://www.wikidata.org/prop/direct/P21>  <http://www.wikidata.org/entity/Q6581072> . "
        + f"<http://www.ex.com/{i+2}> <http://www.wikidata.org/prop/direct/P166> <http://www.ex.com/{i+1}> . "
        + f"<http://www.ex.com/{i+2}> <http://www.wikidata.org/prop/direct/P21>  <http://www.wikidata.org/entity/Q6581097>"
        + "}"
    )


def exec_query(query: str, out: list[str]):
    sparql = SPARQLWrapper("http://localhost:8080/sparql")

    sparql.setMethod("POST")
    sparql.setRequestMethod("postdirectly")
    sparql.setQuery(query)

    assert sparql.queryType is not None

    if sparql.queryType in ("ASK", "SELECT"):
        sparql.setReturnFormat("json")
    elif sparql.queryType in ("DESCRIBE", "CONSTRUCT"):
        sparql.setReturnFormat("turtle")
    else:
        raise ValueError(f"Unknown queryType: {sparql.queryType}")

    res = ""
    try:
        res = sparql.query()
        response = res.convert()
    except QueryBadFormed:
        print("QueryBadFormed")
        sys.exit(1)
    except json.JSONDecodeError:
        print("JSONDecodeError")
        print(res.response.read())
        # print(res.response.read().decode("utf-8"))
        sys.exit(1)

    if isinstance(response, dict):
        # JSON response
        print(json.dumps(response, indent=2))

        if "results" in response:
            # SELECT
            count = len(response["results"]["bindings"])
        elif "boolean" in response:
            # ASK
            count = 1
        else:
            raise ValueError(f"Unknown response format: {response}")

    elif isinstance(response, bytes):
        # TURTLE DESCRIBE response
        response = response.decode()
        count = 0
        for line in response.splitlines():
            print(line)
            count += 1

    else:
        raise ValueError(f"Unknown response format: {response}")

    # print("Results:", count)
    out[0] = f"{count} results"


def exec_update(query: str):
    sparql = SPARQLWrapper("http://localhost:8080/update")

    sparql.setQuery(query)
    sparql.setMethod("POST")
    sparql.setRequestMethod("postdirectly")

    try:
        response = sparql.query()
        print(response)
    except QueryBadFormed:
        print("QueryBadFormed")
        sys.exit(1)


# using List to be able to modify the string
out1 = [""]
out2 = [""]

if __name__ == "__main__":
    delete_list: list[Thread] = []
    for i in range(10):
        t = Thread(target=exec_update, args=[get_delete(i)])
        t.start()
        delete_list.append(t)
    for t in delete_list:
        t.join()

    t1 = Thread(target=exec_query, args=[query, out1])
    t1.start()

    time.sleep(1)

    thread_list: list[Thread] = []

    for i in range(10):
        t = Thread(target=exec_update, args=[get_insert(i)])
        t.start()
        thread_list.append(t)

    time.sleep(10)

    t2 = Thread(target=exec_query, args=[query, out2])
    t2.start()

    t1.join()
    t2.join()

    for t in thread_list:
        t.join()

    # print(f"out1 {out1[0]}")
    # print(f"out2 {out2[0]}")
    with open("out1.txt", "w", encoding="utf-8") as text_file:
        text_file.write(out1[0])

    with open("out2.txt", "w", encoding="utf-8") as text_file:
        text_file.write(out2[0])
