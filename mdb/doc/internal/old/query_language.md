# MillenniumDB Query Language

A query would look like:
```
// This query is asking for the age and name of people
// that knows John having between 60 and 70 years old,
// ordered by their age (ascending) and name (descending).
MATCH (?x :Person)-[Knows]->(John)
WHERE ?x.age >= 60 AND ?x.age <= 70
ORDER BY ?x.name DESC, ?x.age ASC
RETURN ?x.age, ?x.name
LIMIT 1000
```

Let's analyze line by line

- The first 3 lines are comments. A comment starts with a `//` and finish at the end of the line.

- The next line is a MATCH clause. Every query must have a MATCH clause at the beginning. The MATCH clause is followed by a **graph pattern**. To define a graph pattern we need to define some other smaller concepts. First we define the **node pattern**. The most basic **node pattern** looks like this:
    - `()`

    And there are some things you can add to a **node pattern**:

    - A **node identifier** that refers to a specific node or literal.
        | **node type** | example node identifier |
        | :--- | ---: |
        |NamedNode|`(John_Doe)`|
        |AnonymousNode|`(_a123)`|
        |String Literal|`("some string")`|
        |Boolean Literal|`(true)`|
        |Integer Literal|`(123)`|
        |Float Literal|`(3.14)`|

    - If you are not using a **node identifier**, you can add a **variable** to bind the node.
        - `(?x)`

    - A list of labels (after the variable or node identifier if they are present)
        - `(:Label)`
        - `(:Label1 :Label2)`
        - `(?x :Label)`
        - `(?x :Label1 :Label2 :Label3)`
    - A set of properties (a the end)
        - `({key:"value"})`
        - `(?x {key1:"value1", key2:"value2"})`
        - `(?x :Label {key:"value"})`


    Two **node patterns** can be connected to each other vie **edges**, **edges** always have a **direction**:
    - `(?x)->(?y)`
    - `(?y)<-(?z)`

    And similar as in **node patterns**, **edges** may contain other some things:
    - An **edge variable** to bind the edge object:
        - `(?x)-[?e]->(?y)`
    - Or instead of a **edge variable**, a **fixed edge**:
        - `(?x)-[_e123]->(?y)`
    - A **type variable** (after the edge variable or fixed edge if they are present):
        - `(?x)-[:?t]->(?y)`
        - `(?x)-[?e :?t]->(?y)`
    - Or instead of a **type variable**, a **fixed type** (after the edge variable or fixed edge if they are present):
        - `(?x)-[:Type1]->(?y)`
        - `(?x)-[?c :Type2]->(?y)`
    - A set of properties (at the end)
        - `(?x)-[{key:"value"}]->(?y)`
        - `(?x)-[?c :Type {key:"value"}]->(?y)`

    Then a **linear pattern** is a set of one or more **node patterns** linked by edges or property paths (TODO: explain property paths):
    - `(?x :Person)-[:Knows]->(?y)<-[:Knows]-(John)`

    A set of one or more **linear patterns** (separated by comma) forms a **simple graph pattern**
    - `(?x :Person)-[:Knows]->(?y)<-[:Knows]-(John), (?y)-[:LivesIn]->(Chile)`

    Finally, a **graph pattern** is defined as follows:
    - A **simple graph pattern** is a **graph pattern**.
    - If `GP1` and `GP2` are **graph patterns**, `GP1 OPTIONAL { GP2 }` is a **graph pattern**

- TODO: WHERE specification is incomplete. The next line is a WHERE clause. The WHERE clause is optional. The WHERE clause filters the results obtained by the MATCH clause according to certain **condition**. An **atomic condition** looks like:
    - `?x == ?y`
    - `?x == ?y.key`
    - `?x == "literal"`
    - `?x.key == ?y`
    - `?x.key1 == ?y.key2`
    - `?x.key1 == "literal"`

    and `==` may be replaced with the following operators `<, <=, !=, >=, >` and `"literal"` can be replaced with a literal of any type. Complex conditions are defined as:
    - An **atomic condition** is a valid **condition**.
    - If `C1` is a valid condition, `NOT C1` and `(C1)` are valid conditions.
    - If `C1` and `C2` are valid conditions. `C1 AND C2` and `C1 OR C2` are valid conditions.

    The operator precedence is as usual: `()` > `NOT` > `AND` > `OR`.

- The next line is an ORDER BY clause. The ORDER BY clause is optional. You can specify how the order works with the keywords `ASC`/`ASCENDING` and `DESC`/`DESCENDING` after each element. If the order is not specified the default is `ASCENDING`

- The next line is a RETURN clause. Every query must have a RETURN clause. This clause specify which objects or object properties will be returned. Return clauses look like this:
    - `RETURN *`
    - `RETURN ?x`
    - `RETURN ?x.key`
    - `RETURN ?x, ?y.key, ?z`

- The last line is a LIMIT clause. The LIMIT clause is optional. A LIMIT clause gives an upper bound on the number of results returned.

- TODO: link to example queries