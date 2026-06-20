###################
Useful SPARQL Notes
###################



Literal Types mentioned in the SPARQL Specification
###################################################
==============  ========  ========
type            language  datatype
==============  ========  ========
literal         optional  optional
plain literal   optional  no
typed literal   no        yes
simple literal  no        no
==============  ========  ========



RDF Terms
#########
- IRIs
- Blank Nodes
- Literals



Expressions and Variables
#########################
========  ==  ======  ===========  ===========
..        AS  Unsafe  Aggregation  Expressions
========  ==  ======  ===========  ===========
OPTIONAL  ..  X       ..           ..
BIND      X   X       ..           X
GROUP BY  X   X       ..           X
HAVING    ..  ..      X            X
ORDER BY  ..  ..      X            X
VALUES    ..  X       ..           ..
SELECT    X   X       X            X
FILTER    ..  ..      ..           X
========  ==  ======  ===========  ===========



Solution Modifier Order
#######################
ORDER BY, PROJECTION, DISTINCT, REDUCED, OFFSET, LIMIT



Operator Evaluation Order
#########################
GROUPING, AGGREGATES, HAVING, VALUES, SELECT expressions