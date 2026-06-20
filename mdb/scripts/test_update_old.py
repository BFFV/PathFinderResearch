#!/usr/bin/python3

import json
import sys

from SPARQLWrapper import SPARQLWrapper
from SPARQLWrapper.SPARQLExceptions import QueryBadFormed


def exec_query(query: str):
    # print(query)
    # print("-----------------\n")

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

    print("Results:", count)


def exec_update(query: str):
    # print(query)
    print("-----------------\n")

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


if __name__ == "__main__":
    query = """
PREFIX : <http://www.firstPrefix.com/>
SELECT ?o
WHERE  {
    ?s <http://schema.org/test> ?o
}
"""

    update1 = """
INSERT DATA {
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/dateModified> "2022-05-31T15:10:17Z"^^<http://www.w3.org/2001/XMLSchema#dateTime> .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "external_string_test"@new-lang .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "external_string_test"@new-lang2 .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "external_string_test3"@new-lang .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "inl1"@new-lang .

    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "inl2"@new-lang2 .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "a"@l .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "2022-05-31T15:10:17Z"^^<http://www.w3.org/2001/XMLSchema#other> .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "2023-05-31T15:10:17Z"^^<http://www.w3.org/2001/XMLSchema#other> .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "b@la" .
}
"""
    update2 = """
INSERT DATA {
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "2024-05-31T15:10:17Z"^^<http://www.w3.org/2001/XMLSchema#other> .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "b"@l .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "b"@la .
}
"""

    delete1 = """
DELETE DATA {
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "external_string_test"@new-lang .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "external_string_test"@new-lang2 .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "external_string_test3"@new-lang .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "inl1"@new-lang .

    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "inl2"@new-lang2 .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "a"@l .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "2022-05-31T15:10:17Z"^^<http://www.w3.org/2001/XMLSchema#other> .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "2023-05-31T15:10:17Z"^^<http://www.w3.org/2001/XMLSchema#other> .
    <http://www.wikidata.org/entity/Q99501373> <http://schema.org/test> "b@la" .
}
"""

    exec_query(query)
    # exec_update(update1)
    # exec_update(update2)
    # exec_update(delete1)
