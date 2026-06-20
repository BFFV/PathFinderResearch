#############
Client-Server
#############

.. Note::

   This documents assumes you have already created a database.
   If not, check out :ref:`create-database` for instructions.

We implement the typical client/server model, so in order to query a database,
you need to have a server running and then send queries to it.



Running the Server
##################
To run the server use the following command, replacing `<db-directory>` with the database directory::

   mdb server <db-directory>



Executing a Query
#################

.. tabs::

   .. tab:: SPARQL

      The MillenniumDB SPARQL server supports all three query operations specified in the `SPARQL Protocol`_:

         - query via `GET`
         - query via `URL-encoded POST`
         - query via `POST` directly

      So you can make queries with an HTTP request.

      We provide a script that queries using curl.
      You can use the script to make queries as follows::

         bash scripts/query <query-file>

      where `<query-file>` is the path to a file containing a query in SPARQL format.

      .. note::

         The script assumes you use the default server port (1234). Edit the script to use another port.

   .. tab:: MQL

      .. Todo::

         Mention and link to MQL protocol(s)

      Similar as SPARQL, you can make queries with an HTTP request.

      We provide a script that queries using curl.
      You can use the script to make queries as follows::

         bash scripts/query <query-file>

      where `<query-file>` is the path to a file containing a query in SPARQL format.

      .. note::

         The script assumes you use the default server port (1234). Edit the script to use another port.



.. _RDF:             https://www.w3.org/TR/rdf11-primer/
.. _SPARQL:          https://www.w3.org/TR/2013/REC-sparql11-overview-20130321/
.. _Turtle:          https://www.w3.org/TR/turtle/
.. _SPARQL Protocol: https://www.w3.org/TR/2013/REC-sparql11-protocol-20130321/#query-operation