Importing CSVs into MillenniumDB
=======================================

Table of Contents
=======================================
- [Structure of the CSV files](#structure-of-the-csv-files)
  - [Node CSV files](#node-csv-files)
  - [Edge CSV files](#edge-csv-files)
  - [About automatic type detection](#about-automatic-type-detection)
- [Usage of `mdb-import`](#usage-of-mdb-import)


[Structure of the CSV files](#table-of-contents)
=======================================
In order to use the CSV file import functionality, the CSV files that will be supplied need to follow a specific structure.

## Node CSV files
All CSV files must have a header. In this header, it will be specified certain aspects of the data that will be provided. The possible columns are the following:
- `:ID`: This column will provide an ID to identify the nodes during the import process. This is necessary in order to relate nodes to create edges in the graph. Alongside with this, it is possible to create scopes or groups of IDs, by adding `:GROUP` to the column title (for example: `:ID:gp1`, `:ID:x`, `:ID:group_name`). If not specified, the node will be saved into the database, but the importer will not be able to create edges for it later.
- `:LABEL`: This column will add labels to the node being created. On each row, the file can provide one or more labels by using a separator (by default: `;`).
- `name:DATATYPE`: This column will add a property with the name `name`. Optionally, a datatype can be specified by writing it with `:DATATYPE`. If no datatype is supplied, the importer will try to guess while reading the first row. The supported datatypes are:
  - `:STR`: String
  - `:INT`: Integer numbers
  - `:FLOAT`: Decimal numbers (floats)
  - `:DATE`: A date in format `YYYY-MM-DD`
  - `:DATETIME`: A date with time in format `YYYY-MM-DDThh:mm:ss`

For the body of the CSV, each row will correspond to a single node with the properties specified in the header. Any of this properties can be skipped by not writing anything in the corresponding column.

> Example
> ```csv
> propertya1,propertyb1,propertyc1
> propertya2,,propertyc2
> ```

As it was already explained, it is possible to add `:ID` to each node in order to differentiate and reference them during import. It is important that this IDs **do not** repeat in the same scope. If this happens, only the first reference to the ID will be saved and the rest will be discarded during import and not saved onto the database.

> Example where `:ID`s are repeated
> ```csv
> :ID,:LABEL,name
> n1,Member,Nicole
> n2,Member,Matthew
> n2,Member,Mathias
> ```
> In this case, only `n1,Member,Nicole` and `n2,Member,Matthew` will be saved, while `n2,Member,Mathias` will not, as `n2` already existed in the global scope of IDs.

An important thing to note is that IDs will remain for all the import process, which means that IDs must not repeat in any file. To avoid this, the scopes can be used.

> Example where `:ID`s repeat in different scopes
> - `file1.csv`
>   ```csv
>   :ID:1stfloor,:LABEL,name
>   n1,Member,Nicole
>   n2,Member,Matthew
>   ```
> - `file2.csv`
>   ```csv
>   :ID:2ndfloor,:LABEL,name
>   n2,Member,Mathias
>   g1,Guest,Ibrahim
>   g2,Guest,Mary
>   ```
> This will not cause any errors, since both `n2` IDs belong to different scopes.

## Edge CSV files
As already mentioned, all CSV files need to have a header. The possible columns for edge files are the following:
- `:START_ID` and `:END_ID`: Both of these columns specify nodes that will be related. These will have a one way relation that goes from `:START_ID` to `:END_ID` like so:
  <br><div align=center>
    ( `:START_ID` ) --> ( `:END_ID` )
  </div>
- `:UNDIRECTED_ID`: This column is used to specify relations that have no direction. Two of these columns must exist, and there cannot be a `:START_ID` or `:END_ID` columns present. These columns will create a relation that looks like this:
  <br><div align=center>
    ( `:UNDIRECTED_ID` ) --- ( `:UNDIRECTED_ID` )
  </div>
- `:TYPE` *(required when importing using `QuadModel`, a.k.a. `--model quad`)*: This will specify the type of the edge being created. If using `GQL` *(a.k.a. `--model gql`)*, this column will be used as a `:LABEL` instead.
- `:LABEL` *(only compatible with `GQL`)*: This column will add labels to the edge being created.
- `name:DATATYPE`: Similar to nodes files, this column will add a property with the name `name`. Optionally, a datatype can be specified by writing it with `:DATATYPE`. If no datatype is supplied, the importer will try to guess while reading the first row. The same datatypes for nodes are supported for the edges.

As specified before, mixing directed and undirected edge declarations is not allowed, and doing it will result in a fatal error and stop the import process. However, every edge file needs to specify nodes to connect in one of the ways exposed above. If this is not done, the import process will stop.

> Examples of invalid headers:
> ```csv
> :START_ID,:UNDIRECTED_ID,:END_ID
> ```
> This will fail, since an `:UNDIRECTED_ID` is being declared despite the fact that directed edges are being specified.
> ```csv
> propertya,:LABEL,propertyb
> ```
> This will fail, since there is no indication of nodes that will be connected in any way.

Also, since edge labels are only compatible with the `GQL` model, if a `:LABEL` column is added when importing using `--model quad`, this column will be ignored, and a parsing error will appear.

## About automatic type detection
When the CSV header doesn't specify the datatype of a column, the import utility will try to automatically detect the type. This is done when reading the first appearance of the column (this is the first row). Not all datatypes can be found this way. Only `:STR`, `:INT` and `:FLOAT` can be automatically detected.

Considering this, if the column has a type that can be ambiguous or not properly represented by the first row of data, it is recommended to specify in the header the type of said column.


[Usage of `mdb-import`](#table-of-contents)
=======================================
`mdb-import` is the utility used to import data into a new MillenniumDB database. To import CSV data, the utility has to be executed with the following arguments
```shell
mdb csv-import <model> <database path> --nodes <list of files> --edges <list of files>
```
> For example:
> ```shell
> mdb csv-import gql database/my-csv-db --nodes persons.csv stores.csv --edges purchase-info.csv
> ```

Currently, only `GQL` and `QuadModel` have support for importing data from CSV files, and files cannot be given through the standard input.

While edge files are not mandatory for the import process, node files have to be specified, and the import will not start unless nodes are getting imported.