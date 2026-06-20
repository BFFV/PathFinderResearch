#!/usr/bin/env python3

import sys

from SPARQLWrapper import SPARQLWrapper
from SPARQLWrapper.SPARQLExceptions import QueryBadFormed


def send_query(file: str):
    with open(file, "r", encoding="utf-8") as f:
        query = f.read()

    print(query)
    print("-----------------\n")

    sparql = SPARQLWrapper("http://localhost:1234/update")

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
    if len(sys.argv) != 2:
        print("Please pass query filepath")
        sys.exit(1)

    send_query(sys.argv[1])
