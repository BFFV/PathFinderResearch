[Query Rewriting](../developer.md)
================================================================================
Documentation for the implementation of rewrite rules.



Table of Contents
================================================================================
- [RewriteRuleVisitor](#rewriterulevisitor)
- [RewriteRule](#rewriterule)
- [RewriteRules Implemented](#rewriterules-implemented)
- [Sequence Transformation](#sequence-transformation)
- [Unit Tests](#unit-tests)



[RewriteRuleVisitor](#query-rewriting)
================================================================================
For rewriting queries, the class RewriteRuleVisitor is used. To use that
class, it can be initiated with a list of rewrite rules or empty. A rule
can be added by using the method: add_rule, for example:

```
auto rule_rewriter = RewriteRuleVisitor();
rule_rewriter.add_rule<FlattenUnion>();
rule_rewriter.begin_visit(op);
```

Note that, even though rule_rewriter is an OpVisitor, to prevent errors
the method begin_visit should be used, this is because sometimes the
same op will have to be changed, therefore requiring the pointer to the
first op.



[RewriteRule](#query-rewriting)
================================================================================
This abstract class is a template for rule rewriting. There are two important
methods:

- bool is_possible_to_regroup(std::unique_ptr<Op>&)
- std::unique_ptr<Op> regroup(std::unique_ptr<Op>)

is_possible_to_regroup will be recursively called on all ops by
RewriteRuleVisitor. If it ever returns true, regroup is called on the
op that made that return, and RewriteRuleVisitor will change, if it had
not before the value has_rewritten, that determines if it is necessary
to repeat the visitation.

**IMPORTANT**: it is possible that an infinite loop can happen with these
two functions. Two possible cases where this can happen are:

    1. is_possible_to_regroup returns true but no changes happen to the
        op either in that function or in regroup.

    2. there are changes that are reverted by another rewrite rule in
       a RewriteRuleVisitor

There are four helper functions provided in the abstract class RewriteRule,
they are widely used when treating with optionals.



[RewriteRules Implemented](#query-rewriting)
================================================================================
- DecompressAndFilter

`Filter<R1 AND R2>(OP) = Filter<R1, R2>(OP))`

- DistributeFilterOverUnion

`Filter(A1 UNION A2) = Filter(A1) UNION Filter (A2)`

- DistributeJoinUnion

1. `(A1 U A2) JOIN AJ = (A1 JOIN AJ) U (A2 JOIN AJ)`
2. `AJ JOIN (A1 U A2) = (A1 JOIN AJ) U (A2 JOIN Aj)`

- DistributeMinusToUnion

`(A1 U A2) \ Ao= (A1 \ Ao) U (A2 \ Ao) *`

- DistributeOptionalsToUnion

`(A1 U A2) OPT Ao = (A1 OPT Ao) U (A2 OPT Ao)`

- ExistsToSemiJoin

if there is a safevar in OP2 that is in OP1, then:

`OP1 Filter (Exists OP2) = OP1 SEMI JOIN OP2`

- ExprIntoNormalForm

This rewrite rule is the rule that implements all rewriting of
Expressions. Specifically:

  1. Or Distributivity:

    - E1 Or (E2 And E3) = (E1 Or E2) And (E1 Or E3)

  2. Not Distributivity:

    - Not (E1 Or E2) = Not E1 And Not E2
    - Not (E1 And E2) = Not E1 Or Not E2
    - Not Not E = E
    - Not (Exists op) = NotExists var
    - Not (NotExists op) = Exists op

  3. Simplifications of all queries that don't have any variables

  4. `[not] Exists (Op1 OPT Op2) = [not] Exists (Op1)`

- FlattenFilter

`Filter<R1>(Filter<R2>(OP)) = Filter<R1 AND R2>(OP)`

- FlattenUnion

`UNION(UNION(A, B), C) = UNION(A,B,C)`

- LiteralRemoval

  1. `Filter[..., True, ...](op) = Filter[..., ...](op)`
  2. `Filter[..., False, ...](op) = Empty`

- NotExistsToMinus

if there is a safevar in OP2 that is in OP1 then:

  `OP1 Filter (Not Exists OP2) = OP1 MINUS OP2`

- OpNotWellDesignedToWellDesigned

Note that this rewrite rule should ONLY be used if the query is well_designed,
to check if it is use the visitor CheckWellDesigned

 `Optional(...) = WellDesignedOptional(...)`

- PushFilterInto

Using templating, it implements the rules:

- `PushFilterInto<OpJoin>`:

`Filter_R(A1 JOIN A2)  = Filter_R(A1) JOIN A2`

- `PushFilterInto<OpOptional>`

`Filter_R(A1 OPT A2)   = Filter_R(A1) OPT A2`

- `PushFilterInto<OpMinus>`

`Filter_R(A1 MINUS A2) = Filter_R(A1) MINUS A2`



[Sequence Transformation](#query-rewriting)
================================================================================
The ChangeJoinToSequence class, featured in change_join_to_sequence.h
and change_join_to_sequence.cc, converts the tree structure of JOIN
operations into an ordered list of operations called OpSequence. Utilizing
the visitor pattern, it processes and visits operation nodes,
managing JOIN operations within the visit(OpJoin& op_join) function
and accumulating information in the SequenceInformation object. The
SequenceInformation structure stores the ongoing operation sequence, while
the get_pertinent_sequence function constructs the final OpSequence object
after all operations have been visited and transformed. This process
is executed solely within the transform_child_if_necessary function,
which replaces visitations in each visit. The resulting sequence follows
this order:

If a bind was not visited: [merged bgp, ops, services]

If a bind was visited, the bind operator breaks the commutativity,
ensuring the correct resulting sequence. Upon visiting the bind, the
current_bgp, containing all previously visited bgp's information, is
pushed to the potential_sequences op along with the services. The bind
op would be added in the op_join, if necessary. To maintain correctness,
sequences are pushed before the next last_bgp created when the Sequence
is formed within the get_pertinent_sequence information. Consequently,
the order becomes:

[ops, merged bgp_1, services_1, bind_1, ..., merged bgp_n, services_n,
bind_n, ops, merged last_bgp, remaining_services]



[Unit Tests](#query-rewriting)
================================================================================
This section provides an overview of the unit tests for checking
the implementation of rewrite rules in the project. The tests
are written using Catch2 and help ensure the correctness of the
rewrite rules. Detailed examples of these tests can be found in the
catch2_unit_tests/testing_individual_rules.cc file.

The unit tests make use of the same_structure_as and create_op
meta-template programming functions to create different query structures
and compare their internal consistency. This allows for verifying that
the correct query structure is obtained after applying the rewrite rules.

For each rewrite rule, a test case is created using Catch2's TEST_CASE
macro. Within each test case, the original query structure is created
using the create_op function, and a RewriteRuleVisitor object is
instantiated. The specific rewrite rule to be tested is then added to
the visitor using the add_rule method.

After applying the rewrite rule with the visitor's begin_visit method,
the resulting query structure is compared to the expected query structure
using the same_structure_as function. If the structures match, the test
passes, indicating that the rewrite rule has been correctly implemented.

Remember that these are just general guidelines on how the unit
tests are structured for rewrite rules. To see specific examples
and better understand how each test case is written, refer to the
catch2_unit_tests/testing_individual_rules.cc file.
