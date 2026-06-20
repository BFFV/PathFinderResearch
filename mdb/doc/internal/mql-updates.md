# Updates

## Plain creation
- Create a node:
    ```
    INSERT (N1 :L1 {prop:"value"})
    ```

    The full query fails (nothing is inserted) if N1 has the property `prop` with a different value.

- Create an edge:
    ```
    INSERT (N1)-[:T1 {prop:"value"}]->(N2)
    ```

    It always creates a new edge, even if one already exists.

- Create new pattern:
    ```
    INSERT (N1 :L1 {prop:true})-[:Knows]->(N1), (N2 :L1)
    ```

## Plain delete

- Delete node (and its labels and properties):
    ```
    DELETE N1
    ```

- Delete edge (and its properties):
    ```
    DELETE _e1
    ```

### Delete labels
- Delete one label
    ```
    DELETE N1:Label1
    ```

- Delete multiple labels
    ```
    DELETE N1:Label1:Label2
    ```
    alternatively:
    ```
    DELETE N1:Label1, N1:Label2
    ```

- Delete all labels
    ```
    DELETE labels(N1)
    ```

### Delete properties
- Delete node property:
    ```
    DELETE N1.name
    ```

- Delete edge property:
    ```
    DELETE _e1.name
    ```

- Delete all node properties:
    ```
    DELETE properties(N1)
    ```

- Delete all edge properties:
    ```
    DELETE properties(_e1)
    ```


## MATCH ... INSERT

- Insert a label into an existing node
    ```
    MATCH (?x)
    WHERE ?x.name IS NOT NULL
    INSERT (?x :NewLabel)
    ```

- Insert a properties into an existing node
    ```
    MATCH (?x)
    WHERE ?x.name = "Joe"
    INSERT (?x {birth_date:date("2000-01-31")})
    ```
    **TODO:** what should it do when a property exists with different value

- Insert a new edge
    ```
    MATCH (?x)
    WHERE ?x.name = "Joe"
    INSERT (?x)-[:WorksAt { since:(TO_DATE(NOW())) }]->(WorkPlace1)
    ```

## MATCH ... SET
Inserting a property fails if it already exists with another value.
Set is used to create or modify properties.

- Insert or modify a property:
    ```
    MATCH (?x)
    WHERE ?x.name IS STRING
    SET ?x.new_prop = "value", ?x.new_prop2 = "value2"
    ```

    This is different of the above query because it deletes ?x.name
and any other property:

    ```
    MATCH (?x)
    WHERE ?x.name IS STRING
    SET properties(?x) = {new_prop: "value", new_prop2: "value2"}
    ```

    Similarly, you can override the labels of a node:

    ```
    MATCH (?x)
    WHERE ?x.name IS STRING
    SET labels(?x) = {:Label1, :Label2}
    ```


## MATCH ... DELETE

- Delete a node
    ```
    MATCH (N1)-[?e]->(?x)
    DELETE ?x
    ```

- Delete an edge
    ```
    MATCH (N1)-[?e]->(?x)
    DELETE ?e
    ```

- Delete a property
    ```
    MATCH (?x :Person)
    DELETE ?x.name
    ```

    Works as expected even if `?x` is matched with some nodes without the property `name` (those cases are skipped).

- Delete a label
    ```
    MATCH (?x)
    WHERE ?x.name IS STRING
    DELETE ?x:Person
    ```

    Works as expected even if `?x` is matched with some nodes without the label `:Person` (those cases are skipped).
