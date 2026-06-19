# Kuzu

## Setup

**1)** https://kuzudb.com/docusaurus/installation

**2)** https://kuzudb.com/docusaurus/getting-started/cli

## DBMS

**Connect:** ``./kuzu ./db_name`` (inside the Kuzu working directory)

**Version:** ``0.0.6`` (``0.0.8`` throws ``segfault`` errors)

**Python API:** ``kuzu 0.0.6``

## Import DB (WDBench):

*If exists, drop tables for edges (properties) and then for nodes*
```
DROP TABLE Edge;
DROP TABLE Entity;
```

*Create node table for Entity*
```
CREATE NODE TABLE Entity(id STRING, PRIMARY KEY (id));
COPY Entity FROM "../dbs/nodes.csv";
```

*Create edge tables for top 10 properties (PX are all the others)*
```
CREATE REL TABLE P279(FROM Entity TO Entity);
COPY P279 FROM "../dbs/P279.csv";
CREATE REL TABLE P31(FROM Entity TO Entity);
COPY P31 FROM "../dbs/P31.csv";
CREATE REL TABLE P131(FROM Entity TO Entity);
COPY P131 FROM "../dbs/P131.csv";
CREATE REL TABLE P171(FROM Entity TO Entity);
COPY P171 FROM "../dbs/P171.csv";
CREATE REL TABLE P106(FROM Entity TO Entity);
COPY P106 FROM "../dbs/P106.csv";
CREATE REL TABLE P17(FROM Entity TO Entity);
COPY P17 FROM "../dbs/P17.csv";
CREATE REL TABLE P40(FROM Entity TO Entity);
COPY P40 FROM "../dbs/P40.csv";
CREATE REL TABLE P361(FROM Entity TO Entity);
COPY P361 FROM "../dbs/P361.csv";
CREATE REL TABLE P19(FROM Entity TO Entity);
COPY P19 FROM "../dbs/P19.csv";
CREATE REL TABLE P39(FROM Entity TO Entity);
COPY P39 FROM "../dbs/P39.csv";
CREATE REL TABLE PX(FROM Entity TO Entity);  // Didn't load properly
COPY PX FROM "../dbs/PX.csv";
```

*Create edge table for all edges (generic)*
```
CREATE REL TABLE Edge(FROM Entity TO Entity, type STRING);
COPY Edge FROM "../dbs/edges.csv";
```

## Import DB (SNAP):
```
DROP TABLE Knows;
DROP TABLE Person;
CREATE NODE TABLE Person(id STRING, PRIMARY KEY (id));
COPY Person FROM "../dbs/pokec_nodes.csv";
CREATE REL TABLE Knows(FROM Person TO Person);
COPY Knows FROM "../dbs/pokec_edges.csv";
```

## Import DB (Diamond):
```
DROP TABLE E;
DROP TABLE N;
CREATE NODE TABLE N(id STRING, PRIMARY KEY (id));
COPY N FROM "../dbs/diamond_nodes.csv";
CREATE REL TABLE E(FROM N TO N);
COPY E FROM "../dbs/diamond_edges.csv";
```

## Queries
```
All Walks (<30) Check: MATCH p=(:Node {id: "N0"})-[:Edge*1..30]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
All Walks (<30) Enum: MATCH p=(:Node {id: "N0"})-[:Edge*1..30]->(n) RETURN p LIMIT 100000;
Any Shortest Walks (<30) Check: MATCH p=(:Node {id: "N0"})-[:Edge* SHORTEST 1..30]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
Any Shortest Walks (<30) Enum: MATCH p=(:Node {id: "N0"})-[:Edge* SHORTEST 1..30]->(n) RETURN p LIMIT 100000;
All Shortest Walks (<30) Check: MATCH p=(:Node {id: "N0"})-[:Edge* ALL SHORTEST 1..30]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
All Shortest Walks (<30) Enum: MATCH p=(:Node {id: "N0"})-[:Edge* ALL SHORTEST 1..30]->(n) RETURN p LIMIT 100000;
```

## Test

**Database:**
```
N1,N2
N2,N3
N3,N2
```

**Queries:**
```
MATCH p=(:N {id: "N1"})-[:E*5..5]->(n) RETURN p LIMIT 100000;
MATCH p=(:N {id: "N1"})-[:E* SHORTEST 1..5]->(n) RETURN p LIMIT 100000;
MATCH p=(:N {id: "N1"})-[:E* ALL SHORTEST 1..5]->(n) RETURN p LIMIT 100000;
MATCH p=(:N {id: "N1"})-[:E*1..30]->(n) RETURN p LIMIT 100000;
```

**Results:**
```
1 path
2 paths
2 paths
30 paths
```

# Memgraph

## Setup

**1)** https://memgraph.com/docs/memgraph/install-memgraph-on-ubuntu

**2)** https://memgraph.com/docs/memgraph/connect-to-memgraph/mgconsole

## Server

**Start:** ``sudo systemctl start memgraph``

**Stop:** ``sudo systemctl stop memgraph``

**Version:** ``2.11.0``

## Client

**Connect:** ``mgconsole``

**Python API:** ``neo4j 5.13.0``

## Import DB (WDBench):

*If exists, drop nodes/edges*
```
MATCH (n:Entity) DETACH DELETE n;
```

*Load node data*
```
LOAD CSV FROM "/data2/benchmark/bffv/dbs/nodes.csv" NO HEADER AS row
CREATE (n:Entity {id: row[0]});
CREATE INDEX ON :Entity(id);
```

*Load edge data (generic)*
```
LOAD CSV FROM "/data2/benchmark/bffv/dbs/edges.csv" NO HEADER AS row
MATCH (n1:Entity {id: row[0]}), (n2:Entity {id: row[1]})
CREATE (n1)-[:Edge {type: row[2]}]->(n2);
CREATE INDEX ON :Edge;
```

## Import DB (SNAP):
```
MATCH (n:Person) DETACH DELETE n;
LOAD CSV FROM "/data2/benchmark/bffv/dbs/pokec_nodes.csv" NO HEADER AS row
CREATE (n:Person {id: row[0]});
CREATE INDEX ON :Person(id);
LOAD CSV FROM "/data2/benchmark/bffv/dbs/pokec_edges.csv" NO HEADER AS row
MATCH (n1:Person {id: row[0]}), (n2:Person {id: row[1]})
CREATE (n1)-[:Knows]->(n2);
CREATE INDEX ON :Knows;
```

## Import DB (Diamond):
```
MATCH (n:N) DETACH DELETE n;
LOAD CSV FROM "/data2/benchmark/bffv/dbs/diamond_nodes.csv" NO HEADER AS row
CREATE (n:N {id: row[0]});
CREATE INDEX ON :N(id);
LOAD CSV FROM "/data2/benchmark/bffv/dbs/diamond_edges.csv" NO HEADER AS row
MATCH (n1:N {id: row[0]}), (n2:N {id: row[1]})
CREATE (n1)-[:E]->(n2);
CREATE INDEX ON :E;
```

## Queries
```
All Trails Check: MATCH p=(:Node {id: "N0"})-[:Edge*]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
All Trails Enum: MATCH p=(:Node {id: "N0"})-[:Edge*]->(n) RETURN p LIMIT 100000;
Any Shortest Walks Check: MATCH p=(:Node {id: "N0"})-[:Edge* BFS]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
Any Shortest Walks Enum: MATCH p=(:Node {id: "N0"})-[:Edge* BFS]->(n) RETURN p LIMIT 100000;
All Shortest Walks Check: MATCH p=(:Node {id: "N0"})-[:Edge* ALLSHORTEST (r, n | 1)]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
All Shortest Walks Enum: MATCH p=(:Node {id: "N0"})-[:Edge* ALLSHORTEST (r, n | 1)]->(n) RETURN p LIMIT 100000;
```

## Test

**Database:**
```
N1,N2
N2,N3
N3,N2
```

**Queries:**
```
MATCH p=(:N {id: "N1"})-[:E*5]->(n) RETURN p LIMIT 100000;
MATCH p=(:N {id: "N1"})-[:E*BFS 5]->(n) RETURN p LIMIT 100000;
MATCH p=(:N {id: "N1"})-[:E*ALLSHORTEST 5 (r, n | 1)]->(n) RETURN p LIMIT 100000;
```

**Results:**
```
0 paths
0 paths
2 paths (due to lower bound):
(:N {id: "N1"})-[:E]->(:N {id: "N2"})
(:N {id: "N1"})-[:E]->(:N {id: "N2"})-[:E]->(:N {id: "N3"})
```

# NebulaGraph

## Setup

**1)** https://docs.nebula-graph.io/3.5.0/2.quick-start/2.install-nebula-graph/

**2)** https://docs.nebula-graph.io/3.5.0/nebula-console/

**3)** https://docs.nebula-graph.io/3.5.0/2.quick-start/3.connect-to-nebula-graph/

**4)** https://docs.nebula-graph.io/3.5.0/2.quick-start/3.1add-storage-hosts/

**5)** https://docs.nebula-graph.io/3.5.0/nebula-importer/use-importer/

**6)** https://docs.nebula-graph.io/3.5.0/2.quick-start/4.nebula-graph-crud/

## Server

**Start:** ``sudo /usr/local/nebula/scripts/nebula.service start all``

**Stop:** ``sudo /usr/local/nebula/scripts/nebula.service stop all``

**Version:** ``3.5.0``

## Client

**Import DB:** ``./nebula-importer --config config_file.yaml`` (inside the Nebula working directory)

**Connect:** ``./nebula-console -port 9669 -u root`` (inside the Nebula working directory)

**Choose DB:** ``USE db_name;``

**Update Index:** ``REBUILD TAG/EDGE INDEX index_name;``

**Python API:** ``nebula3-python 3.4.0``

## Queries (MATCH)
```
All Trails Check: MATCH p=(:Node {id: "N0"})-[:Edge*]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
All Trails Enum: MATCH p=(:Node {id: "N0"})-[:Edge*]->(n) RETURN p LIMIT 100000;
Any Shortest Trails Check: MATCH p=shortestPath((:Node {id: "N0"})-[:Edge*]->(:Node {id: "N3"})) RETURN p LIMIT 100000;
Any Shortest Trails Enum: MATCH p=shortestPath((:Node {id: "N0"})-[:Edge*]->(n)) RETURN p LIMIT 100000;
All Shortest Trails Check: MATCH p=allShortestPaths((:Node {id: "N0"})-[:Edge*]->(:Node {id: "N3"})) RETURN p LIMIT 100000;
All Shortest Trails Enum: MATCH p=allShortestPaths((:Node {id: "N0"})-[:Edge*]->(n)) RETURN p LIMIT 100000;
```

## Queries (FIND PATH)
```
All Trails Check: FIND ALL PATH FROM "N0" TO "N3" OVER Edge YIELD path AS p | LIMIT 100000;
Any Shortest Trails Check: FIND SINGLE SHORTEST PATH FROM "N0" TO "N3" OVER Edge YIELD path AS p | LIMIT 100000;
All Shortest Trails Check: FIND SHORTEST PATH FROM "N0" TO "N3" OVER Edge YIELD path AS p | LIMIT 100000;
All Acyclic Check: FIND NOLOOP PATH FROM "N0" TO "N3" OVER Edge YIELD path AS p | LIMIT 100000;
```

## Queries (GO)
```
All Walks Check: GO min TO max STEPS FROM "N0" OVER Edge WHERE properties($$).id == "N3" YIELD edge AS e | LIMIT 100000;
All Walks Enum: GO min TO max STEPS FROM "N0" OVER Edge YIELD edge AS e | LIMIT 100000;
```

## Test

**Database:**
```
N1,N2
N2,N3
N3,N2
```

**Queries:**
```
MATCH p=(:Node {id: "N1"})-[:E*5]->(n) RETURN p LIMIT 100000;
MATCH p=shortestPath((:Node {id: "N1"})-[:E*1..5]->(n)) RETURN p LIMIT 100000;
MATCH p=allShortestPaths((:Node {id: "N1"})-[:E*1..5]->(n)) RETURN p LIMIT 100000;
```

**Results:**
```
0 paths
2 paths
2 paths
```

# Neo4j

## Setup

**1)** https://github.com/MillenniumDB/WDBench/blob/remove_cross_product/README.md

## Server

**Start:** ``sudo bin/neo4j console`` (inside the Neo4j directory)

**Version:** ``5.12.0``

## Client

**Import DB:**

```
sudo bin/neo4j-admin database import full \
--nodes=Person="../../dbs/pokec_nodes_header.csv,../../dbs/pokec_nodes.csv" \
--relationships=Knows="../../dbs/pokec_edges_header.csv,../../dbs/pokec_edges.csv" \
--delimiter "," --array-delimiter ";" --skip-bad-relationships true pokec
```

**Connect:** ``sudo bin/cypher-shell`` (inside the Neo4j directory)

**Python API:** ``neo4j 5.13.0``

## Queries (MATCH)
```
All Trails Check: MATCH p=(:Node {id: "N0"})-[:Edge*]->(:Node {id: "N3"}) RETURN p LIMIT 100000;
All Trails Enum: MATCH p=(:Node {id: "N0"})-[:Edge*]->(n) RETURN p LIMIT 100000;
Any Shortest Walks Check: MATCH p=shortestPath((:Node {id: "N0"})-[:Edge*]->(:Node {id: "N3"})) RETURN p LIMIT 100000;
Any Shortest Walks Enum: MATCH p=shortestPath((:Node {id: "N0"})-[:Edge*]->(n)) RETURN p LIMIT 100000;
All Shortest Walks Check: MATCH p=allShortestPaths((:Node {id: "N0"})-[:Edge*]->(:Node {id: "N3"})) RETURN p LIMIT 100000;
All Shortest Walks Enum: MATCH p=allShortestPaths((:Node {id: "N0"})-[:Edge*]->(n)) RETURN p LIMIT 100000;
```
