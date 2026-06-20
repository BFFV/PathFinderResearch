# Query rewriting

## RewriteRuleVisitor

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

## RewriteRule

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

## Concrete RewriteRules implemented: Ops

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
