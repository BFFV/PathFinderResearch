Quantified path patterns
########################

Path patterns can be used along with quantifiers. The variables within a quantified path can bind to a list of elements instead of a single element.

.. csv-table::
   :header-rows: 1
   :escape: \

   Quantifier, Description
   `+`      , 1 or more repetitions.
   `*`      , 0 or more repetitions.
   `?`      , 0 or 1 repetitions.
   `{m\,n}` , Between `m` and `n` repetitions.
   `{m}`    , Exactly `m` repetitions.
   `{m\,}`  , `m` or more repetitions.
   `{\,n}`  , Between 0 and `n` repetitions.

A path pattern with a quantifier must have a minimum path length greater than 0. The path length is computed as follows:

* The minimum path length of a node `n` is 0.
* The minimum path length of an edge `e` is 1.
* The minimum path length of a concatenation of the path patterns `p1` and `p2` is the sum of their minimum path lengths.
* The minimum path length of an union between the path patterns `p1` and `p2` is the minimum of minimum path lengths between `p1` and `p2`


For example, the path length of ``(a)-[e]->(b)-[f]->(c)`` is 2, since it is the concatenation of 3 node patterns and 2 edge patterns. The rule of the minimum length prevents us to write queries like this:

.. code-block:: gql

   MATCH p = (a)+
   RETURN a


Examples
********

In this example, we search for paths between John and Jane of length between 0 and 4.

.. code-block:: gql

   MATCH p = ({name:"John"})-[:Follows]->{,4}({name:"Jane"})
   RETURN p

We can use union operators as well. For example, we use the union to combine the results of directed edges and undirected edges.

.. code-block:: gql

   MATCH p = ({name:"John"})(->|~){2,5}({name:"Jane"})
   RETURN p


Unbounded repetition
********************

When using repetition on patterns, the number of results may infinite if the repetition is unbounded. A pattern has unbounded repetition if its quantifier is `*`, `+` or `{n,}`. For example, we search for paths of length at least 1.

.. code-block:: gql

   MATCH p = ({name:"John"})-[:Follows]->+({name:"Jane"})
   RETURN p

To guarantee that the number of results is finite, the statement containing a pattern with unbounded repetition must specify a path mode, a path search prefix or the keyword `DIFFERENT EDGES`  to restrict the result. In the next section, we will see path prefixes.
