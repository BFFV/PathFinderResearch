
# B+ Tree Compression

## Compression

The compression is based on the redundancy of the records. Since some of the bytes are used to store the types and the records are sorted, there are some bytes that are the same in every record. The idea of the compression is to store the redundant bytes separately, so that the size of each record stored is equal to the size of the non-redundant bytes.

This compression is only applied to the leaves of the B+ tree.

## Summary

The following table shows the structure of a leaf in the B+ tree. **N** represents how many items in the triple are stored and **S** represents the the number of redundant bytes and can be obtained by adding the bits in the bitset.

| Section        | Description                              |                            Size |
| -------------- | ---------------------------------------- | ------------------------------: |
| RecordCount    | Total number of records.                 |                         4 bytes |
| NextLeaf       | Pointer to the next leaf.                |                         4 bytes |
| Bitset         | Bitset to mark the redundant positions.  |                         N bytes |
| RedundantBytes | The redundant bytes of the records.      |                         S bytes |
| Records        | The non-redundant bytes for each record. | RecordCount * (N * 8 - S bytes) |
