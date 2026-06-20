Importing RDF/XML into MillenniumDB
=======================================

Table of Contents
=======================================
- [Important notes](#important-notes)
  - [`TODOs`](#todos)
  - [Working Syntax](#working-syntax)
  - [Uniprot Testing](#uniprot-testing)
- [Usage of `mdb-import`](#usage-of-mdb-import)

Important notes
=======================================
## `TODOs`
Certain characteristics of the RDF/XML standard do not work at the moment. According to the [W3C Standard](https://www.w3.org/TR/rdf-syntax-grammar/), the following aspects of the format are missing:
- [XML Literals (`rdf:parseType="Literal"`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-XML-literals)
- [Omitting Blank Nodes (`rdf:parseType="Resource"`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-parsetype-resource)
- [Omitting Nodes (Property Attributes on an empty Property Element)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-property-attributes-on-property-element)
- [Typed Node Elements](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-typed-nodes)
- [Container Membership Property Elements (`rdf:li` and `rdf:_n`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-list-elements)
- [Collections (`rdf:parseType="Collection"`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-parsetype-Collection)
- `owl:Ontology` handling, according to [the following forum](https://stackoverflow.com/questions/27727845/what-does-owlontology-rdfabout-xmlbase-in-an-ontology-mean)

## Working Syntax
At the moment, the following parts of the standard are working
- Sections 2.1 - 2.6 ([`example07.rdf`](https://www.w3.org/TR/2004/REC-rdf-syntax-grammar-20040210/example07.rdf) uses all syntax points described and works in MillenniumDB).
- [Languages (`xml:lang`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-languages)
- [Typed literals (`rdf:datatype`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-datatyped-literals)
- [Identifying blank nodes (`rdf:nodeID`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-blank-nodes)
- [Abbreviating URIs (`rdf:ID` and `xml:base`)](https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-ID-xml-base)

## Uniprot Testing
This has proven to be enough to import data from the uniprot dataset, including the following databases
- [`diseases.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/diseases.rdf.xz)
- [`enzyme.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/enzyme.rdf.xz)
- [`journals.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/journals.rdf.xz)
- [`locations.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/locations.rdf.xz)
- [`pathways.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/pathways.rdf.xz)
- [`proteomes.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/proteomes.rdf.xz)
- [`taxonomy.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/taxonomy.rdf.xz)
- [`tissues.rdf`](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/tissues.rdf.xz)

These, and more testing files can be found [here](https://ftp.uniprot.org/pub/databases/uniprot/current_release/rdf/)

Usage of `mdb-import`
=======================================
`mdb-import` is the utility used to import data into a new MillenniumDB database. To import data from an RDF/XML file into an RDF model database, `mdb-import` has to be executed with the following command
```shell
mdb import <XML Source File> <database path>
```

A prefix file can also be given in order to use IRI compression, similar to RDF importing in other formats (such as TURTLE `.ttl`). This file has to be supplied using the option `--prefix <file>`.

> For example:
> ```shell
> mdb import data.xml data/my_rdf_db --prefix data/my_prefixes.txt
> ```

The RDF/XML file provided has to be properly formatted according to the standard. Not following this may result in warnings appearing in the output log or in the worst case, a fatal error that stops the import process.