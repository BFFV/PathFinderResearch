[SPARQL Datatypes](../developer.md)
================================================================================
This documents the SPARQL datatype implementation details.


Table of Contents
================================================================================
- [Date/Time](#datetime)
- [Decimal](#decimal)
- [Float](#float)
- [Integer](#integer)
- [IRI](#iri)
- [Literal with Datatype](#literal-with-datatype)
- [Literal with Language](#literal-with-language)
- [Simple Literal](#simple-literal)



[Date/Time](#sparql-datatypes)
================================================================================
```
"2010-10-10Z"^^xsd:date
"10:10:10Z"^^xsd:time
"2010-10-10T10:10:10Z"^^xsd:dateTime
"2010-10-10T10:10:10Z"^^xsd:dateTimeStamp
```

Different sub-types
--------------------------------------------------------------------------------
MillenniumDB handles the following date/time types:
| type              | date | time | timezone |
|-------------------|------|------|----------|
| xsd:date          | yes  | no   | optional |
| xsd:time          | no   | yes  | optional |
| xsd:dateTime      | yes  | yes  | optional |
| xsd:dateTimeStamp | yes  | yes  | yes      |

These are defined in [Data Types, RDF 1.2](https://www.w3.org/TR/rdf-concepts/#xsd-datatypes).
In this document we will use **date/time** to refer to all four together.
To refer to individual types we use the official name without the xsd prefix.


Important semantics
--------------------------------------------------------------------------------
- Currently we do not support **fractional seconds**, they are simply dropped.
- We allow the **year 0**. Year 1 is 1 CE, year 0 is 1 BCE and year -1 is 2 BCE, etc.
  [(dateTime, XSD 1.1 Part 2)](https://www.w3.org/TR/xmlschema11-2/#dateTime)
- Dates with an absolute value of the year that does not fit into **14 bits** (> 16383) are stored as just the year. Any time and timezone information is dropped. We call this **low precision**.


Format
--------------------------------------------------------------------------------
Depending on if the absolute value of the year fits into 14 bits (max 16383), we use one of the following two formats.
The generic type means any of the date/time types.
The subtype refers to the four individual date/time types.

### Low precision
If the year does not fit into 14 bits (> 16383), then we store just the years, losing time and timezone information.

```text
FROM MSB to LSB:
generic type :  4 bits (defined in object_id.h)
subtype      :  2 bits (defined in object_id.h)
unused       :  2 bits
sign         :  1 bit  (year sign, 0:positive, 1:negative)
low_pres     :  1 bit  (in this case 1, 0:normal precision, 1:low precision)
year         : 54 bits (number of years)
```

### Normal precision
If the year fits into 14 bits (max 16383), then we store the components normally.

```text
FROM MSB to LSB:
generic type :  4 bits (defined in object_id.h)
subtype      :  2 bits (defined in object_id.h)
unused       :  2 bits
sign         :  1 bit  (year sign, 0:positive, 1:negative)
low_pres     :  1 bit  (in this case 0, 0:normal precision, 1:low precision)
year         : 14 bits (number of years)
month        :  4 bits (number of months)
day          :  5 bits (number of days)
hour         :  5 bits (number of hours)
minute       :  6 bits (number of minutes)
second       :  6 bits (number of seconds)
has tz       :  1 bit  (0:no tz, 1: has tz)
tz sign      :  1 bit  (0:positive, 1:negative)
tz hour      :  5 bits (number of timezone hours)
tz min       :  6 bits (number of timezone minutes)
tz0_z        :  1 bit  (timezone zero lexical representation, 0:+/-xx:xx, 1:Z)
```


Comparison and equality
--------------------------------------------------------------------------------
For comparison and equality testing we use ToTl (Time on Timeline) as described in [Time on timeline, XML Schema 1.1](https://www.w3.org/TR/xmlschema11-2/#vp-dt-timeOnTimeline). ToTl handles leap years, leap seconds are ignored.
- If both values have or do not have timezones then the comparison or equality test is done using ToTl.
- If one value has a timezone and the other not we compare ToTl of both. If the the difference is less than 14 hours the result is ambiguous and would depend on the timezone of the date/time without a defined timezone. In case of ambiguity we return an error, otherwise we return the correct result.
- For the purpose of comparisons and equality testing dateTime and dateTimeStamp are the same type.
- Comparisons (<, <=, >, >=) of different types return an error ("null").
- For equality testing (=, !=) different types are never equal, they are always different.


References
--------------------------------------------------------------------------------
- [Data types, RDF](https://www.w3.org/TR/rdf-concepts/#xsd-datatypes)
- [Comparisons, xpath](https://www.w3.org/TR/xpath-functions/#comp.datetime)
- [Time on timeline, XML Schema 1.1](https://www.w3.org/TR/xmlschema11-2/#vp-dt-timeOnTimeline)
- [dateTime, XML Schema](https://www.w3.org/TR/xmlschema-2/#dateTime)
- [dateTime, XML Schema 1.1](https://www.w3.org/TR/xmlschema11-2/#dateTime)



[Decimal](#sparql-datatypes)
================================================================================
```
-23.32
"0.43"^^xsd:decimal
```

Codification
--------------------------------------------------------------------------------
### Inlined
```text
[ type ][ sign ][ digits ][ separator position ]

type               :  1 byte
sign               :  1 bit  (1 means negative, 0 positive)
digits             : 51 bits (whole number digits without separator)
separator position :  4 bits (separator distance from the right)
```

- The separator position is the distance of the separator from the right (e.g. if the separator is `3`, the number could be `0.123`, `1234.555`, `100.654`, etc).

### Extern
```text
[ type ][ sign ][ size ][ exponent ][ digits ]

type     :    1 byte
sign     :    1 bit   (1 means negative, 0 positive)
size     :    7 bits  (number of bytes used for digits)
exponent :    8 bits  (int8_t holding the base 10 exponent)
digits   : size bytes (4 bits per digit, little endian, no leading/trailing zeros)
```

- If there are an uneven number of digits, then the last byte of the digits holds only one digit in the least significant `4 bits`, (e.g. `0x05`), and the last digits is zero. So for an uneven number of digits one trailing zero is allowed, which is ignored.
- The value is equal to `(-1 or 1, depending on sign) * (whole number represented by digits) * (10 ^ exponent)`.



[Float](#sparql-datatypes)
================================================================================
Codification
--------------------------------------------------------------------------------
```text
[ type ][ unused ][ number ]

type   : 1 byte
unused : 3 bytes (unused bytes)
number : 4 bytes (Number in IEEE-754 standard floating-point format)
```

- The `double` type is transformed to `float`, losing precision.



[Integer](#sparql-datatypes)
================================================================================
Codification
--------------------------------------------------------------------------------
```text
[ type ][ number ]

type   :  1 byte
number :  56 bits
```

- If the number is positive, the type will be `MASK_POSITIVE_INT` and the number is stored normally.
- If the number is negative, the type will be `MASK_NEGATIVE_INT` and the number is stored as `abs(number)`.
- If the user imports or queries with an integer that uses more than `56 bits` on its codification, it will be converted into a `Extern Decimal`.



[IRI](#sparql-datatypes)
================================================================================
Codification
--------------------------------------------------------------------------------
### Inlined
```text
[ type ][ prefix id ][ characters ]

type       :  1 byte
prefix id  :  1 byte  (prefix index in rdf_catalog.prefixes)
characters :  6 bytes (IRI suffix characters)
```

- The `characters` start at the most significant byte and the trailing unused bytes are filled with `\0`.
- If the `characters` are composed of exactly `6 bytes`, it won't store the null terminator `'\0'`.

### Extern
```text
[ type ][ prefix id ][ unused ][ extern id ]

type      :  1 byte
prefix id :  1 byte  (prefix index in rdf_catalog.prefixes)
unused    :  4 bits
extern id : 44 bits (identifier in string manager or temporal manager)
```



[Literal with Datatype](#sparql-datatypes)
================================================================================
Codification
--------------------------------------------------------------------------------
### Inlined
```text
[ type ][ datatype id ][ unused ][ characters ]

type         :  1 byte
datatype id  : 12 bits (datatype index in rdf_catalog.datatypes)
unused       :  4 bits
characters   :  5 bytes (literal characters)
```

- The `characters` start at the most significant byte and the trailing unused bytes are filled with `\0`.
- If the `characters` are composed of exactly `5 bytes`, it won't store the null terminator `'\0'`.

### Extern
```text
[ type ][ datatype id ][ extern id ]

type        :  1 byte
datatype id : 12 bits (datatype index in rdf_catalog.datatypes)
extern id   : 44 bits (identifier in string manager or temporal manager)
```

We can have up to 2^12 - 1 (4095) different datatypes in the catalog. If more datatypes are used in the database, we use a reserved value that indicates the extern id has the language at the end of the string (separated by `^`)

[Literal with Language](#sparql-datatypes)
================================================================================
Codification
--------------------------------------------------------------------------------
### Inlined
```text
[ type ][ language id ][ unused ][ characters ]

type         :  1 byte
language id  :  12 bits (datatype index in rdf_catalog.datatypes)
unused       :  4 bits
characters   :  5 bytes (literal characters)
```

- The `characters` start at the most significant byte and the trailing unused bytes are filled with `\0`.
- If the `characters` are composed of exactly `5 bytes`, it won't store the null terminator `'\0'`.

### Extern
```text
[ type ][ language id ][ extern id ]

type        :  1 byte
language id : 12 bits (datatype index in rdf_catalog.datatypes)
extern id   : 44 bits (identifier in string manager or temporal manager)
```

We can have up to 2^12 - 1 (4095) different languages in the catalog. If more languages are used in the database, we use a reserved value that indicates the extern id has the language at the end of the string (separated by `@`)

[Simple Literal](#sparql-datatypes)
================================================================================
Codification
--------------------------------------------------------------------------------
### Inlined
```text
[ type ][ characters ]

type       :  1 byte
characters :  7 bytes (string characters)
```

- The `characters` start at the most significant byte and the trailing unused bytes are filled with `\0`.
- If the `characters` are composed of exactly `7 bytes`, it won't store the null terminator `'\0'`.

### Extern
```text
[ type ][ unused ][ extern id ]

type      :  1 byte
unused    : 12 bits
extern id : 44 bits (identifier in string manager)
```
