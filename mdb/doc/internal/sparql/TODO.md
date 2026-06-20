[TODO](../developer.md)
================================================================================



Table of Contents
================================================================================
- [Misc](#misc)
- [Architecture](#architecture)
- [Sequence](#sequence)
- [Service](#service)



[Misc](#todo)
================================================================================



[Architecture](#todo)
================================================================================
- `REGEX` and `REPLACE` expressions use the standard library implementation of
  regular expressions, which does not follow SPARQL 1.1 exactly. For example,
  flags specified in SPARQL 1.1 are not implemented. Also the syntax and
  semantics may not be identical.

- `TupleCollection` performance is important, however, it currently does a
  lot of unnecessary std::vector allocations.

- The `Decimal` implementation could be a lot faster. Currently a lot of
  allocations are made because vectors are used all over the place. This was
  done to allow variable precision, essentially turning `Decimal` into a base-10
  floating-point number. In hindsight, a simpler and more performant
  implementation would be to use arrays of fixed size to represent fixed
  precision decimals.

- Currently all aggregation functions are calculated separately, even if they
  are identical. This could be optimized to calculate every identical
  expression only once. One way to implement this could be some sort of
  expression normalization in combination with a hash-table. Maybe this could
  also be expanded into common sub-expression elimination for both normal and
  aggregation expressions, so that repeated sub-expressions are only calculated
  once.

- `SELECT DISTINCT` always uses hashing, to improve performance already
  ordered sequences could be taken advantage of to avoid the hashing.

- Currently always two `OrderById`s are used when we have an `ORDER BY` clause
  and a `GROUP BY` clause. However, this is not always necessary. In some cases
  the order of the first `OrderById` already coincides with the order of the
  second `OrderById`.

- Currently `GROUP BY` saves all variables that are in `var_manager`, this could
  be optimized to only save necessary variables.



[Sequence](#todo)
================================================================================
- Better Plan: The current implementation is correct, but the best left-deep
  plan is not always the best plan. Association can be introduced to optimize
  costs. For example, ((A-B)-(C-D)).

- Finally, there is no cost optimization at all. There is in the BGP operator
  but no between operators within SERVICE.



[Service](#todo)
================================================================================
- Better JOIN Implementation for Service: The current implementation makes one
  request per binding when a join variable is fixed in a SERVICE using values.
  A solution would be to use something like a block nested loop join, which
  would fix multiple bindings at once in the service.

- Consume: try to perform the handshake and all the request faster.

- Consume: send the request and receive the response in a buffer. Code a
  pipeline implementation read of the results to manage huge responses.

- Consume: If an added VALUES does not match the VALUES within SERVICE we know
  it is a empty result and could avoid sending the request.

- Parse: Extract methods 'ObjectId to string' or 'string to ObjectId'.

- Parse: Improve the state logic in response parser automata. Instead of having
  many states, the current ObjectId::Mask can be stored.

- Parse: Add TSV request parsing to the parsing time for a better estimation o
  the duration.

- Parse (or operate): Due to the overhead in the request, if the assigned
  variables do not change, then the reset should not send a new request.
  The assigned variables do not have an order but in practice can save a lot
  of cost.