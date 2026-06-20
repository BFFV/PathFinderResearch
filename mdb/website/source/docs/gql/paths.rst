Paths
#####

GQL queries allows us to reference and return paths. For this, we use path variables.


Path variables
**************

Path variables allows us to reference paths. For example, we can return all edges and their source and destination nodes using the variable `p`.

.. code-block:: gql

   MATCH p = ()-[]->()
   RETURN p

There are functions that we can use in path variables. For example, `PATH_LENGTH` or `ELEMENTS`.

.. code-block:: gql

   MATCH p = ({id:13})->({id:15})
   RETURN PATH_LENGTH(p)

Using the `PATH_LENGTH` function does is not really useful in this query, because the path has fixed length. Sometimes we want to find more complex paths, so then we can use unions, multiset alternation and quantifiers.


Parenthesized paths
*******************

Paths can have parenthesized subpaths.

For example, in this query we have the author node and the edge in a parenthesis.

.. code-block:: gql

   MATCH ((a:Author)-[:IsAuthorOf]->)(:Book)
   RETURN a

Parenthesized path patterns can also contain a where clause.

.. code-block:: gql

   MATCH ((e:Editor)~(a:Author) WHERE e.born = a.born)->(:Book)
   RETURN a, e

This will be useful when using the union operator and quantifiers.


Union
*****

We can use the path pattern union operator to combine the results from its operands.

For example, we can obtain in a single query all books with their authors and authors that know each other.

.. code-block:: gql

   MATCH p = (:Author)->(:Book) | (:Author)~[:Knows]~(:Author)
   RETURN p

In this example, we use the union operator inside a parenthesis.

.. code-block:: gql

   MATCH p = (:Author)~[:Knows]~((:Author) | (:Editor))
   RETURN p


Multiset Alternation
********************

The multiset alternation operator combine the results similarly to the union operator, but duplicate results are removed from the working table.

For example, we can use the union operator `|` to obtain all authors and all editors, but if there is an author that is also an editor, then the node will appear twice in the resulting table.

.. code-block:: gql

   MATCH (a:Author) | (a:Editor)
   RETURN a

To  avoid getting duplicates, we can use the multiset alternation operator `|+|`.

.. code-block:: gql

   MATCH (a:Author) |+| (a:Editor)
   RETURN a
