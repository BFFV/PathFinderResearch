###########
Text Search
###########



Overview
########
The text search is implemented using the following parts:

1. A trie to lookup the search term
2. A B+ tree to associate data to the trie nodes
3. A table containing the actual data associated with trie
   nodes using the B+ tree.

A search consist of tokenizing and normalizing the query string.
The query tokens are than looked up in the trie and the node_id
of the matching node is obtained. The node_id is then looked-up
in the B+ tree to obtain a table pointer, pointing to a row in
the table. The table row contains the data that is returned.
If the query string consists of multiple tokens then we do the
previous lookup for each token and materialize the table pointers.
A join is then performed on the materialized iters.


Main Classes
############


TextSearch
==========
This is the main interface of the text search.
It is used as follows:

1. It is initialized with the path of the database directory
   and pointers to a tokenize and normalize function.
2. An index is created using `create_index` if not using a preexisting one.
3. Next the index has to be loaded using `load_index`.

Now that we have loaded an index we can do any of the following:

- Index a predicate and add it to the the currently loaded index
  using `index_predicate`.
- Iterate over all the string contained in the index trie using
  `get_iter_list`.
- Make different type of searches using `match`, `prefix`,
  `math_error` and `prefix_error`.
- Output the trie with `print_trie` in the DOT format to
  visualize it using Graphviz.


Trie
====
This class is the main interface to the trie used to look up search terms.
It also defines the tree metadata that is saved to disk.
The fields are saved to page one starting at offset 0.

The header containing the metadata consists of three fields.
The Trie class contains pointers to them (ending with `_ptr`).

1. `end_page_pointer`, 5 bytes: A :term:`page pointer` to the first unused space in
   the :term:`page file`. It is used to obtain more space for new nodes or
   when having to relocate nodes so they can grow.
2. `root_page_pointer` 5 bytes: A :term:`page pointer` to the root node of the trie.
   Initially the root node is placed just after the header, but when it grows
   if will have to be relocated. This field allows us to always find the root
   node.
3. `next_id`, 5 bytes: Each trie node has an associated :term:`node id`, which are
   assigned incrementally starting at 0. This field contains the next free Id that
   can be assigned to a new node.


Node
====
Every node of the trie is represented by this class. A node contains a 
potentially empty string and each transition from a parent node to a child
node has an associated char. Every contains the following data in the
page file, in order:

1. `node_id`, 5 bytes: A stable identifier of the node. Identifiers are
   assigned incrementally starting at 0. `next_it` of the trie stores the
   next identifier to be used.
2. `capacity`, 2 bytes: The space allocated for the node, this allows nodes
   to grow without having to relocate them every time.
3. `document_count`, 1 bytes: The number of :term:`documents <document>`
   associated with the node. The counter is capped at 255 if more documents
   are added than it can hold. This is currently only used for debugging and
   could be removed, since presence of documents (and count) can be inferred
   from how many entries the node has in the B+ tree.
4. `str_len`, 1 byte: the length of the string stored in the node.
5. `child_count`, 1 byte: the number of children the node has.
6. `string`, <str_len> bytes: The string itself contained in the node.
7. `children`, <child_count> * (1+5) bytes: Each entry represents a child
   and consists of the transition char (1 bytes) and the :term:`page pointer`
   to the child (5 bytes).

The space for nodes is a power of 2, starting with 16 bytes. When adding a
child to a node and there is enough space the node can be simply updated.
If the space is not enough we allocate new space with double the capacity
(checking the trie garbage first). The page pointer from the parent node
to the resized node has to be update to point to the new location. The old
space is added to the trie garbage so it can be reused later.


Table
=====
The table holds the actual items indexed. It has a header starting at
offset 0 of the page 0 with the following fields in order:

1. `column_count`, 8 bytes: indicates the number of columns of the table.
2. `eng_page_pointer`, 8 bytes: A page :term:`page pointer` to the the
   first unused space.

Following the header are rows of :term:`ObjectIds <ObjectId>`.



Glossary
########
.. glossary::

   Page pointer
      A pointer to a specific position in a page. It is the byte offset from
      the beginning of the page file, and this implicitly contains the page
      number and page offset. The page pointer of trie nodes change when they
      are relocating after growing.

   Page file
      The file containing the pages, which is accessed using the BufferManager.

   Node id
      An id assigned to each trie node. They are stable, once a node is crated
      it never changes it's id. This way we can reference the trie nodes from
      the B+ tree.

   Document
      The content that is indexed. A document (such as a paragraph of text)
      can have multiple search terms that are added do the trie.
