[Developer Documentation](../README.md)
=======================================

Table of Contents
=================
- [Other Documents](#other-documents)
- [VSCode Setup](#vscode-setup)
- [Running Tests](#running-tests)
- [Useful SPARQL Notes](#useful-sparql-notes)


[Other Documents](#developer-documentation)
===========================================
- [SPARQL Datatypes](sparql/datatypes.md)
- [SPARQL Architecture](sparql/architecture/architecture.md)
- [Query Rewriting](sparql/query_rewriting.md)
- [TODO](sparql/TODO.md)


[Clang Setup](#developer-documentation)
=======================================
Sometimes when you install clang via the package manager, it won't be recognized by cmake as a c++ alternative. Here is an example to change gcc to clang as the default c++ compiler in Ubuntu/Linux Mint (you may use different versions instead of `clang-15` and `libstdc++-12-dev`)
```
sudo apt install clang-15 libstdc++-12-dev
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-15 10
sudo update-alternatives --config c++
```


[VSCode Setup](#developer-documentation)
========================================
If VSCode does not prompt you to install the recommended extensions you can go to the
extensions section in the sidebar and search for `@recommended` to show all the
extensions recommended by the workspace.



[Running Tests](#developer-documentation)
=========================================
To run the tests Python >= 3.8 with venv is needed.\
On Debian and Ubuntu based distributions venv is not included
with Python and has to be installed separately:
```
apt-get -y install python3 python3-venv
```

To compile and run all the tests run:
```
./scripts/run-tests
```

[Useful SPARQL Notes](#developer-documentation)
===============================================
Literal Types mentioned in the SPARQL Specification
---------------------------------------------------
|      type      | language | datatype |
|----------------|----------|----------|
| literal        | optional | optional |
| plain literal  | optional | no       |
| typed literal  | no       | yes      |
| simple literal | no       | no       |


RDF Terms
---------
- IRIs
- Blank Nodes
- Literals


Expressions and Variables
-------------------------
|          | AS  | Unsafe | Aggregation | Expressions |
|----------|:---:|:------:|:-----------:|:-----------:|
| OPTIONAL |     |   X    |             |             |
| BIND     |  X  |   X    |             |      X      |
| GROUP BY |  X  |   X    |             |      X      |
| HAVING   |     |        |      X      |      X      |
| ORDER BY |     |        |      X      |      X      |
| VALUES   |     |   X    |             |             |
| SELECT   |  X  |   X    |      X      |      X      |
| FILTER   |     |        |             |      X      |


Solution Modifier Order
-----------------------
 `ORDER BY`, `PROJECTION`, `DISTINCT`, `REDUCED`, `OFFSET`, `LIMIT`


Operator Evaluation Order
-------------------------
`GROUPING`, `AGGREGATES`, `HAVING`, `VALUES`, `SELECT expressions`
