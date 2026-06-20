# HNSW Index

The HNSW index lets user create indexes based on a RDF predicate to efficiently search and retrieve nodes from the database given a tensor.

## Create a HNSW Index

The HSNW Index can be created via an update request and requires a name `string`, a predicate `iri`, the dimension of the tensors that will be inserted, the maximum number of edges for each node, the maximum number of candidates and the metric `string`, which must be one of the following:

- "cosineDistance"
- "euclideanDistance"
- "manhattanDistance"

An update query for creating the index would be something like the following:

```sparql
PREFIX ex: <http://www.example.com/>
CREATE HNSW INDEX "example" ON ex:tensor
WITH DIMENSION 128
     MAX_EDGES 8
     CANDIDATES 8
     METRIC "cosineDistance"
```

## Querying a HNSW Index

If you have no previous information about the database maybe you want to know which Text Search Indexes are available. SPARQL provide the following query to get each name of the available HNSW Search Indexes and their metadata:

```text
SHOW HNSW INDEX
```

For querying in SPARQL we have the reserved IRI `http://millenniumdb.com/procedure/hnswSearch` (or for short `mdbproc:hnswSearch`) to instantiate a HNSW Search Index Scan. It receives as arguments:

- `(str)` HNSW Search Index name
- `(tensor | var)` Query tensor
- `(int)` Number of neighbors
- `(int)` Number of candidates
- `(var, optional)` Distance projection
- `(var, optional)` Tensor projection

For example:

```sparql
PREFIX mdbproc: <http://millenniumdb.com/procedure/>
SELECT ?subject ?distance ?tensor
WHERE {
    ?subject mdbproc:hnswSearch ("example" "[1,2,3]"^^mdbtype:tensorFloat 100 10000 ?distance ?tensor) .
}
```

## TODO

- Use ObjectId instead of copying the tensor
- Concurrency
- Updates (inserts)
- On-disk storage (inteligente)
- Updates (deletes)
- Unfixed query
