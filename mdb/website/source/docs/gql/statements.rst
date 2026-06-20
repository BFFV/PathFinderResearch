Statements
##########

GQL queries are composed by a sequence of statements. These statements are evaluated one by one and each of them updates the working table, which is a temporary table that stores partial results.

The statements are the following.

* `RETURN`
* `MATCH`
* `LET`
* `FOR`
* `GROUP BY`
* `OFFSET`
* `LIMIT`


RETURN
******

The return statement evaluates expressions and returns them as results.

Examples
========

In this query, we pass a numerical constant as an expression.

.. code-block:: gql

   RETURN 100

In this query, we pass a mathematical expression.

.. code-block:: gql

   RETURN (1 + 2 + 3 + 4) / 4

In this query, we pass an string.

.. code-block:: gql

   RETURN "Hello GQL!"

We can also return multiple expressions and use the keyword `AS` to define aliases.

.. code-block:: gql

   RETURN 100 AS number, "Hello GQL!" AS message

These examples only return constants, which does not seem to be that useful. To search for nodes and edges in the graph database, we need the statement `MATCH`.

Group By
========

The return statement can also contain a group by clause. This clause groups the records by a value.

For example, in this query we obtain the average age of authors for each location.

.. code-block:: gql

   MATCH (a:Author)
   RETURN a.location AS location, AVG(a.age) GROUP BY a.location


MATCH
*****

The match statement defines a graph pattern to match. It can also have a where clause to filter results.

Examples
========

In this query, we declare variables in graph pattern and return them in the return statement.

.. code-block:: gql

   MATCH (a:Author)-[:IsAuthorOf]->(b:Book)
   RETURN a, b

We can also obtain the properties of an element.

.. code-block:: gql

   MATCH (a:Author)
   RETURN a.name, a.born

We can add a where clause in the match statement to filter results.

.. code-block:: gql

   MATCH (a: Author) WHERE a.born <= 1950
   RETURN a.name, a.born


LET
***

The let statement allows us to define variables to use in other statements. It adds a column to the working table.

Examples
========

In this query, we define the variables `x` and `y`.

.. code-block:: gql

   LET x = 1, y = 2
   RETURN x, y

We can define variables and use them in the match statement.

.. code-block:: gql

   LET author_name = "Jane"
   MATCH (a:Author {name: author_name})
   RETURN a

We can define expressions as well.

.. code-block:: gql

   MATCH (a:Author)~(b:Author)
   LET age_avg = (a.age + b.age) / 2
   RETURN a.name, b.name, age_avg


FOR
***

The for statement is used to unnest a list or a group variable and join it with the working table.

Examples
========

In this example, we define a list and return each element as a different result.

.. code-block:: gql

   FOR elem IN [1, 2, 3]
   RETURN elem

In this example, we join the elements of the list with the authors. As a result, we obtain 3 results for each author.

.. code-block:: gql

   MATCH (a:Author)
   FOR elem IN [1, 2, 3]
   RETURN a, elem

In this example, the variable `e` has a group degree of reference, so `e` can bind to one or more edges (see section :ref:`section-types`.). Using a for statement, we get each edge as a different result.

.. code-block:: gql

   MATCH ()-[e]->+()
   FOR elem IN e
   RETURN elem


FILTER
******

The filter statement allows us to filter records in the working table that do not pass the expression conditions.

Examples
========

In this example, we filter the results where `a` is equal to `b`.

.. code-block:: gql

   MATCH (a:Author)~(b:Author)
   FILTER a.id <> b.id
   RETURN a, b

We can use the let statement define variables and use them in the filter statement.

.. code-block:: gql

   MATCH (b:Book)
   LET bid = b.id
   FILTER bid >= 23
   RETURN b


ORDER BY
********

The order by statement sorts the working table by the specified expressions.

Examples
========

In this query, we obtain all the authors sorted by name.

.. code-block:: gql

   MATCH (a:Author)
   ORDER BY a.name
   RETURN a.name

By default, the sort is ascendent. We can explicitly specify the order with `ASC` and `DESC`.

.. code-block:: gql

   MATCH (a:Author)
   ORDER BY a.name DESC
   RETURN a

If there are null results, we can specify whether they appear first or last using `NULLS FIRST` and `NULLS LAST`.

.. code-block:: gql

   MATCH (a:Author)
   ORDER BY a.name DESC NULLS LAST
   RETURN a


OFFSET
******

The offset statement skips the specified number of records in the working table. It is equivalent to the keyword `SKIP`.

Examples
========

In this query, we skip the first 3 results obtained.

.. code-block:: gql

   MATCH (a:Author)
   OFFSET 3
   RETURN a

In this query, we skip the 3 oldest authors using the order by statement.

.. code-block:: gql

   MATCH (a:Author)
   ORDER BY a.born ASC
   SKIP 3
   RETURN a


LIMIT
*****

The limit statement limits the number of records in the working table.


Examples
========

In this query, we obtain at most 10 authors.

.. code-block:: gql

   MATCH (a:Author)
   LIMIT 10
   RETURN a
