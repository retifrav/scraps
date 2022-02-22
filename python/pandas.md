## Pandas

<!-- MarkdownTOC -->

- [Getting started](#getting-started)
- [Define a table](#define-a-table)
- [Growing the table](#growing-the-table)
    - [Add rows](#add-rows)
    - [Add columns](#add-columns)
- [Modify cells values](#modify-cells-values)
- [Filter table](#filter-table)
- [Concat tables](#concat-tables)

<!-- /MarkdownTOC -->

### Getting started

- [10 minutes tutorial](https://pandas.pydata.org/docs/user_guide/10min.html)
- [Comparison with SQL](https://pandas.pydata.org/docs/getting_started/comparison/comparison_with_sql.html)

``` sh
$ pip install pandas
```

``` py
import pandas
```

### Define a table

``` py
someTable = pandas.DataFrame(
    {
        "a": pandas.Series(dtype="float"),
        "b": pandas.Series(dtype="float"),
        "c": pandas.Series(dtype="float"),
        "d": pandas.Series(dtype="str"),
    }#,
    #index=[0]
)
```

### Growing the table

#### Add rows

``` py
for x in [1,2]:
    some = []
    for y in [3,4,5]:
        some.append(x * y)
    someRow = pandas.DataFrame(
        {
            "a": [some[0]],
            "b": [some[1]],
            "c": [some[2]],
            "d": ["ololo"]
        },
        index = [x]
    )
    someTable = someTable.append(someRow)

print(someTable)

#      a    b     c      d
# 1  3.0  4.0   5.0  ololo
# 2  6.0  8.0  10.0  ololo
```

#### Add columns

``` py
print(someTable)

#      a    b     c      d
# 1  3.0  4.0   5.0  ololo
# 2  6.0  8.0  10.0  ololo

someArray = numpy.array([4.5, 2.36])
someTable["new-column"] = someArray

print(someTable)

#      a    b     c      d  new-column
# 1  3.0  4.0   5.0  ololo        4.50
# 2  6.0  8.0  10.0  ololo        2.36
```

### Modify cells values

``` py
someTable.at[1,"c"] = 33
someTable.at[2,"b"] = 22
someTable.at[2,"d"] = "some"

print(someTable)

#      a     b     c      d
# 1  3.0   4.0  33.0  ololo
# 2  6.0  22.0  10.0   some
```

### Filter table

Exclude rows which have `d` column value starting with `olo`:

``` py
tableFiltered = someTable[~someTable["d"].str.startswith("olo")]

print(tableFiltered)

#      a     b     c     d
# 2  6.0  22.0  10.0  some
```

### Concat tables

``` py
table1 = pandas.DataFrame(
    {
        "a": pandas.Series(dtype="float"),
        "b": pandas.Series(dtype="float"),
        "c": pandas.Series(dtype="float"),
        "d": pandas.Series(dtype="str"),
    }
)

table2 = table1.copy()

for x in [1,2]:
    some = []
    for y in [3,4,5]:
        some.append(x * y)
    someRow = pandas.DataFrame(
        {
            "a": [some[0]],
            "b": [some[1]],
            "c": [some[2]],
            "d": ["ololo"]
        },
        index = [x]
    )
    table1 = table1.append(someRow)

print(table1)

#      a    b     c      d
# 1  3.0  4.0   5.0  ololo
# 2  6.0  8.0  10.0  ololo

for x in [3,4]:
    some = []
    for y in [6,7,8]:
        some.append(x * y)
    someRow = pandas.DataFrame(
        {
            "a": [some[0]],
            "b": [some[1]],
            "c": [some[2]],
            "d": ["fuuuu"]
        },
        index = [x]
    )
    table2 = table2.append(someRow)

print(table2)

#       a     b     c      d
# 3  18.0  21.0  24.0  fuuuu
# 4  24.0  28.0  32.0  fuuuu

frames = []
frames.append(table1)
frames.append(table2)
rez = pandas.concat(frames)

print(rez)

#       a     b     c      d
# 1   3.0   4.0   5.0  ololo
# 2   6.0   8.0  10.0  ololo
# 3  18.0  21.0  24.0  fuuuu
# 4  24.0  28.0  32.0  fuuuu
```
