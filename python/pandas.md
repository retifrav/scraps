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
- [Filter out groups that have certain count](#filter-out-groups-that-have-certain-count)

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

### Filter out groups that have certain count

``` py
import pandas
import random

someTable = pandas.DataFrame(
    {
        "a": pandas.Series(dtype="float"),
        "b": pandas.Series(dtype="float"),
        "c": pandas.Series(dtype="float"),
        "d": pandas.Series(dtype="str"),
    }
    #index=[0]
)

someTable = pandas.concat(
    [
        someTable,
        pandas.DataFrame({"a":5,"b":6,"c":7,"d":"smth"}, index=[0])
    ]
).reset_index(drop=True)

someTable = pandas.concat(
    [
        someTable,
        pandas.DataFrame({"a":1,"b":2,"c":3,"d":"another"}, index=[0])
    ]
).reset_index(drop=True)

for x in ["odin", "dva", "tri"]:
    some = []
    for y in [4,5,6,7]:
        some.append(2 * y)
    someRow = pandas.DataFrame(
        {
            "a": [round(some[0] * random.random(), 1)],
            "b": [round(some[1] * random.random(), 1)],
            "c": [round(some[2] * random.random(), 1)],
            "d": ["some"]
        },
        index = [x]
    )
    someTable = pandas.concat([someTable, someRow])


someTable = pandas.concat(
    [
        someTable,
        pandas.DataFrame({"a":11,"b":22,"c":33,"d":"another"}, index=[0])
    ]
).reset_index(drop=True)

print(someTable)

#       a     b     c        d
# 0   5.0   6.0   7.0     smth
# 1   1.0   2.0   3.0  another
# 2   2.9   6.5   0.4     some
# 3   0.7   4.5   2.8     some
# 4   0.1   3.6  10.4     some
# 5  11.0  22.0  33.0  another

listForFiltering = someTable.groupby("d").size().loc[lambda x: x > 2].index
print(someTable.query("d in @listForFiltering.values"))

#      a    b     c     d
# 2  2.9  6.5   0.4  some
# 3  0.7  4.5   2.8  some
# 4  0.1  3.6  10.4  some
```
