############
Sphinx Notes
############

Internal Sphinx notes and experiments.

.. code:: mql

  MATCH (?x)->(?y)
  RETURN ?x, ?y
  MatCH (?x)->(?y) "MATCH \t asdf"
  SIMPLE


Top-level section
#################


Sub-section
===========

Sub-sub-section
---------------

.. This is a comment
  on multiple lines


Documentation links
###################
- `Sphinx docs <https://www.sphinx-doc.org/en/master/usage/index.html>`_
- `reST docs <https://docutils.sourceforge.io/rst.html>`_

References
##########

- code_: reference which only works in the same document.
- `CODE <code_>`_: reference with custom title.
- Sub-section_: reference to a sub-section by name.
- `Code blocks`_: reference to a target with spaces.
- This is an auto-numbered footnote [#]_
- This is another auto-numbered footnote [#]_
- This is an auto-symbol footnote [*]_
- This is another auto-symbol footnote [*]_
- This is a labeled footnote [#label]_
- This is a labeled footnote [PaperX]_
- This is a sphinx extension that creates a reference that works across documents: :ref:`code`.
  ``:ref:`` is a role, 
- You can also customize the title: :ref:`title <code>`.
  ``:ref:`` is a role, 
- `URL <http://google.com>`_: inline link with custom title.
- http://google.com inline link
- `a link`_ to google


.. _a link: http://google.com

.. [#] This is a footnote
.. [#] This is a footnote
.. [*] This is a footnote
.. [*] This is a footnote
.. [#label] This is a footnote
.. [PaperX] This is a footnote



.. _code:

Code blocks
###########


Using directives
================

.. code-block:: turtle

  @prefix : <http://example.org/> .
  @prefix foaf:       <http://xmlns.com/foaf/0.1/> .

  :a foaf:name "Alan" .
  :a foaf:mbox <mailto:alan@example.org> .
  :b foaf:name "Bob" .
  :b foaf:mbox <mailto:bob@example.org> .
  :c foaf:name "Claire" .
  :c foaf:mbox <mailto:claire@example.org> .
  :a foaf:knows :b .
  :b foaf:knows :c .


.. code-block:: python

  def function(num: int):
    if num < 3:
      return True
    else:
      yield 2 * num


.. code-block:: c++

  int my_fun(double num) {
    if (num > 4) {
      return 3;
    } else if (num.is_something()) {
      return -1;
    }
  }


.. code-block:: sparql

  SELECT *
  WHERE {
    ?s ?p ?o .
    FILTER (STRLEN(?o) > 4)
  }
  ORDER BY ?o
  LIMIT 10


.. py:function::
  enumerate(sequence)
  enumerate(sequence[, start=0])

  Return an iterator that yields tuples of an index and an item of the
  *sequence*. (And so on.)

.. cpp:function::
  int mult(int a, int b)

  Return a * b.


.. index:: pair: inline; block


Inline
======
- without syntax highlighting: :code:`var = obj.method(3 * 5, "string") if True else "stuff"`.
- with syntax highlighting: :python:`var = obj.method(3 * 5, "string") if True else "stuff"`.
- with syntax highlighting: :mql:`FILTER ?x = "string"`.


.. index:: pair: literal; block



Literal blocks
##############
A literal block follow::

  literal block
         indentation is preserved
    notice the single colon True

::

  another block, without preceding paragraph

Some paragraph ::

  A block following a paragraph without a colon



Admonitions
###########

.. admonition:: Admonition

  admonition

.. note::

  note

.. hint::

  hint

.. important::

  important

.. tip::

  tip

.. attention::

  attention

.. caution::

  caution

.. warning::

  warning

.. danger::

  danger

.. error::

  error
