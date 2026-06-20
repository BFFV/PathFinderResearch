# Tensors in MillenniumDB

MillenniumDB extends SPARQL by introducing tensor literals, which are arrays of numeric values of the same type. Tensors enable efficient mathematical operations within queries, making them a powerful tool for data processing.

## Defining a Tensor Literal

To declare a tensor in MillenniumDB, the datatype must be specified using the special prefix `<https://mdb.imfd.cl/type#>` (or the shorthand `mdbtype:`) followed by the type of numeric values the tensor holds. Currently, the supported types are:

- `mdbtype:tensorFloat`: Represents a tensor of single-precision floating-point numbers
- `mdbtype:tensorDouble`: Represents a tensor of double-precision floating-point numbers

Tensor literals can be used in both import files and SPARQL queries. They consist of a comma-separated list of numeric values enclosed in square brackets, followed by the datatype. The whitespaces inside the brackets are optional. Here are some examples:

```text
# integers stored as floats
"[1,2,3]"^^mdbtype:tensorFloat

# explicit decimal values
"[1.2, 1.3, 1.16]"^^mdbtype:tensorDouble

# scientific notation
"[1e-3, 1e100, 1.3e-16]"^^mdbtype:tensorFloat
```

## Usage and operations

Tensors can be used in all the contexts where a regular literal can be used in SPARQL. If the types mismatch or the operations that requier multiple tensors do not have the same dimension, as other SPARQL expressions do the result will be `null`.

The result of the expression will always be the most precise tensor if any. For example:

- `"[1,2]"^^mdbtype:tensorFloat + "[1,1]"^^mdbtype:tensorDouble -> "[2,3]"^^mdbtype:tensorDouble`.
- `mdbfn:pow("[3,3]"^^mdbtype:tensorFloat, "2.0"^^xsd:double) -> "[9,9]"^^mdbtype:tensorFloat`.

Let `tensor` be any type of tensor and `scalar` be any numeric value (e.g. `float`, `double`, `xsd:decimal`, `xsd:integer`). Here is a list with the supported operations:

### Adapted from SPARQL

```text
# unary sign
+(tensor) -> tensor
-(tensor) -> tensor

# arithmetic operations
# let op = (+ | - | * | /)
tensor op tensor -> tensor
tensor op scalar -> tensor
scalar op tensor -> tensor

# SPARQL functions
ABS(tensor)   -> tensor
CEIL(tensor)  -> tensor
FLOOR(tensor) -> tensor
ROUND(tensor) -> tensor # with SPARQL 1.1 semantic
```

### MillenniumDB extended tensor functions

To use this MillenniumDB extended functions, the datatype must be specified using the special prefix `<https://mdb.imfd.cl/function#>` (or the shorthand `mdbfn:`)

```text
mdbfn:sqrt(tensor)        -> tensor
mdbfn:pow(tensor, scalar) -> tensor

mdbfn:dot(tensor, tensor) -> scalar
mdbfn:sum(tensor)         -> scalar


mdbfn:cosineSimilarity(tensor, tensor)  -> scalar in range [-1.0, 1.0]

# metrics
mdbfn:cosineDistance(tensor, tensor)    -> scalar in range [0.0,  2.0]
mdbfn:euclideanDistance(tensor, tensor) -> scalar in range [0.0,  inf]
mdbfn:manhattanDistance(tensor, tensor) -> scalar in range [0.0,  inf]
```

### TODO

- Optimize tensor operations with the use of SIMD intrinsics
- Implement more tensor datatypes (e.g. `byte`, `boolean`)
- Optimize the tensor allocations as most of the times there will be at most two tensors at a time
