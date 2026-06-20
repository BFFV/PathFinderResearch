[Quad Model](../README.md)
================================================================================


Creating a database
--------------------------------------------------------------------------------
A QuadModel database can only be queried with [MQL](old/query_language.md).

Before creating the database you need an input file (**TODO:** link to doc specifying the format)

The command to create a new database looks like this:
`build/Release/bin/create_db_mql <path to data file> <path to new database directory>`

For instance, if you want to create a database in `dbs/example` using the example we provide in `data/example-db.txt` you can run:
`build/Release/bin/create_db_mql data/example-db.txt dbs/example`

To delete a database just delete the created directory.


Querying a database
--------------------------------------------------------------------------------
We implement the typical a client/server model, so in order to query a database, you need to have a server running and then send queries to it.

### Run the server
`build/Release/bin/server_mql <path to database directory>`

### Execute a query
`build/Release/bin/query_mql < <path to query file>`



Quad Model with Docker
================================================================================
`docker build -t mdb.backend .`


Create Database
--------------------------------------------------------------------------------
change directory into the folder where the .txt file to be imported is (created db folder will be there too)
- `docker run --volume "$(pwd)":/data mdb.backend /MillenniumDB/build/Release/bin/create_db_mql /data/example-db.txt /data/docker_example`


Run server
--------------------------------------------------------------------------------
- `docker run --volume "$(pwd)":/data --network="host" mdb.backend /MillenniumDB/build/Release/bin/server_mql /data/docker_example`


Execute a query
--------------------------------------------------------------------------------
- `build/Release/bin/query_mql < [path/to/query_file]`