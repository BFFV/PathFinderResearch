##########################
Docker Client-Server Setup
##########################

To run `mdb server` in one container and connect to it from
another container there are two alternatives:

1. If you are using Linux the simplest way is **using the host network**.
2. An alternative is **creating a Docker network**, this should work on all systems.

In this document we describe the Docker setup itself,
for instructions on how to use MillenniumDB in a client-server setup check out :doc:`client-server`.

Using the Host Network
######################

The simples method is using `--network host` as follows::

   docker run --rm -it \
      --volume "$PWD/data:/data" \
      --network host \
      imfd/millenniumdb

Inside the container console, execute the server::

   mdb server <db_folder>

.. note::

  if your database folder is in `./data/foo` then replace `<db_folder>` with `foo`.

After launching the server you can query via HTTP from any container within the same network. Example:

::

   docker run --rm \
      --network host \
      alpine/curl -sS \
      -H "Content-Type:application/sparql-query" \
      -X POST http://localhost:1234/sparql \
      --data-binary "SELECT * WHERE {?a ?b ?c} LIMIT 100"


Creating a Docker Network
#########################

First we have to create the network::

   docker network create mdb_net

Then we can run the server container as follows::

   docker run --rm -it \
      --volume "$PWD/data:/data" \
      --network mdb_net \
      --hostname mdb_server \
      imfd/millenniumdb

After creating any databases and launching `mdb server` we can
run another container in the same network and use the hostname given (`mdb_server`) to send the HTTP request::

   docker run --rm -it \
      --network mdb_net \
      alpine/curl -sS \
      -H "Content-Type:application/sparql-query" \
      -X POST http://mdb_server:1234/sparql \
      --data-binary "SELECT * WHERE {?a ?b ?c} LIMIT 100"

To remove the network run::

   docker network rm mdb_net
