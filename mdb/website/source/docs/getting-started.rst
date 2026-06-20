###############
Getting Started
###############
MillenniumDB currently supports two database models:

- RDF_, which can be queried using SPARQL_
- Quad, which can be queried using MQL

In this document we will describe how to get up and running using `mdb cli`.

For instructions on how to use the client-server model check out :doc:`/docs/client-server`
after reading :ref:`create-database`.



.. _create-database:

Creating a Database
###################
To create a database you need a data-file.

.. todo ::

   Describe and link to `.qm` format.

- For a Quad database the data-file should have a `.qm` extension.
- For a RDF database the file should be in the Turtle_ format and have a `.ttl` extension.


To import data from a data-file and create a database use the `mdb import` command::

   mdb import <data-file> <db-directory>


- `<data-file>` is the path to the file containing the data to import.
- `<db-directory>` is the path of the directory where the new database will be created.

.. Tip::

   If your data-file does not have a matching extension and you do not want to change
   the extension you can pass the explicit `-m,--model` option with `quad` or `rdf`::

      mdb import -m <model> <data-file> <db-directory>



Prefix Definitions (Optional)
=============================
Using a prefixes-file is optional, but helps reduce the space occupied by IRIs in the database.
MillenniumDB generates an :term:`OID` for each prefix, and when importing IRIs into the database replaces
any prefixes with IDs. For large databases this can save a significant amount of space.

The prefixes-file should contain one prefix per line,
with each line consisting of a prefix alias and the prefix itself::


   http://www.myprefix.com/
   https://other.prefix.com/foo
   https://other.prefix.com/bar

To use a prefixes-file you can pass it to the `mdb import` command using the `--prefixes` options::

   mdb import --prefixes <prefixes-file> <data-file> <db-directory>



Using the Interactive CLI
#########################
After creating a database you can use the interactive CLI interface.

All you need to do is launch `mdb cli`, passing it the database, as follow::

   mdb cli <db-directory>

The database model is detected automatically.
Upon launching the CLI it will show a message with instructions.
Depending on the database model you can start executing SPARQL or MQL queries.



Removing a Database
###################
To remove a database simply delete the directory::

   rm -r <db-directory>



.. _RDF:             https://www.w3.org/TR/rdf11-primer/
.. _SPARQL:          https://www.w3.org/TR/2013/REC-sparql11-overview-20130321/
.. _Turtle:          https://www.w3.org/TR/turtle/
.. _SPARQL Protocol: https://www.w3.org/TR/2013/REC-sparql11-protocol-20130321/#query-operation