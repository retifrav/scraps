## Pandas

<!-- MarkdownTOC -->

- [Getting started](#getting-started)
- [Declare a table](#declare-a-table)
    - [Validating schema with pandera](#validating-schema-with-pandera)
    - [Auto-generated values](#auto-generated-values)
        - [NaNs](#nans)
        - [Randoms](#randoms)
- [Growing the table](#growing-the-table)
    - [Add rows](#add-rows)
    - [Add columns](#add-columns)
    - [Concat tables](#concat-tables)
- [Modify cells values](#modify-cells-values)
- [Filter table](#filter-table)
- [Filter out groups that have certain count](#filter-out-groups-that-have-certain-count)
- [Drop row with maximum value in a column](#drop-row-with-maximum-value-in-a-column)
- [pandas.Series to numpy.ndarray](#pandasseries-to-numpyndarray)
- [Find duplicate rows](#find-duplicate-rows)

<!-- /MarkdownTOC -->

### Getting started

- [10 minutes tutorial](https://pandas.pydata.org/docs/user_guide/10min.html)
- [Comparison with SQL](https://pandas.pydata.org/docs/getting_started/comparison/comparison_with_sql.html)

``` sh
$ pip install pandas
```

``` py
import pandas
import numpy
```

### Declare a table

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

#### Validating schema with pandera

Even though you've [declared](#declare-a-table) a table, the types won't hold - if you'll insert a row that will have a different type in one of the columns, that original table type in that column will simply get changed (*which is retarded, although convenient for many*).

To introduce strict(*er*) validation, you'd need to use yet another package - [pandera](https://pandera.readthedocs.io/):

``` py
someTableSchema = pandera.DataFrameSchema(
    {
        "a": pandera.Column(int),
        "b": pandera.Column(float),
        "c": pandera.Column(str)
    }
)

someRow1 = pandas.DataFrame(
    {
        "a": 1,
        "b": 1.3,
        "c": "ololo"
    },
    index=[1]
)
someRow2 = pandas.DataFrame(
    {
        "a": 2.1,
        "b": 6.1,
        "c": "fuuu"
    },
    index=[2]
)
someTable = pandas.concat([someRow1, someRow2])

someTableSchema(someTable)
```

As column `a` expects integer, validation will fail like this:

```
SchemaError: expected series 'a' to have type int64, got float64
```

#### Auto-generated values

##### NaNs

``` py
someTable = pandas.DataFrame(
    numpy.nan,
    index=[1, 2, 3], # without being specified, index will be auto-generated as [0, 1, 2]
    columns=["a", "b", "c"]
)

print(someTable)

#     a   b   c
# 1 NaN NaN NaN
# 2 NaN NaN NaN
# 3 NaN NaN NaN
```

##### Randoms

``` py
rng = numpy.random.default_rng()
someTable = pandas.DataFrame(
    rng.integers(0, 20, size=(3, 5)),
    #index=[1, 2, 3],
    columns=["a", "b", "c", "d", "e"]
)

print(someTable)

#     a  b  c   d   e
# 0   1  7  2   3  12
# 1   4  9  0   3  18
# 2  11  1  6  18   6
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

#### Concat tables

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

### Filter out groups that have certain count

``` py
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

### Drop row with maximum value in a column

``` py
rng = numpy.random.default_rng()
someTable = pandas.DataFrame(
    # numpy.random.randint(0, 20, size=(11, 5)) # that seems to be old(?)
    rng.integers(0, 20, size=(11, 5)),
    columns=list("ABCDE")
)
print(someTable)

newTable = someTable.drop(someTable["C"].idxmax())
print(newTable)
```

If there is more than one row that contains the maximum value in the specified column, then only the first row with that value will be removed, all other will remain in the table.

### pandas.Series to numpy.ndarray

One can read the following plain-text file:

```
# wavelength    flux
2.00    4.889163E+03
2.10    4.127248E+03
2.20    3.365333E+03
2.30    2.603418E+03
```

into a `numpy.ndarray` like this:

``` py
fluxes = numpy.genfromtxt(
    "./moses.txt",
    dtype=float,
    skip_header=1,
    names=["lambda", "flux"]
)

print(type(fluxes))
print(fluxes.dtype)

# <class 'numpy.ndarray'>
# [('lambda', '<f8'), ('flux', '<f8')]
# [(  2. , 4.889163e+03) (  2.1, 4.127248e+03) (  2.2, 3.365333e+03) ...
#  (799.8, 8.069750e+06) (799.9, 8.062498e+06) (800. , 8.055245e+06)]
```

And if there is a `pandas.DataFrame`, each row (*`pandas.Series`*) of which has the same structure as that `numpy.ndarray` from above, then it is possible to convert those into the same `numpy.ndarray` with the same columns names like this:

``` py
pnd = pandas.read_pickle("/path/to/fluxes.pkl")

tpls = [(lmbd, flx) for lmbd, flx in pnd.iloc[0].items()]
dt = numpy.dtype("float64, float64")
fluxes = numpy.array(tpls, dtype=dt)
fluxes.dtype.names = ["lambda", "flux"]

print(type(fluxes))
print(fluxes.dtype)

# <class 'numpy.ndarray'>
# [('lambda', '<f8'), ('flux', '<f8')]
```

### Find duplicate rows

- <https://pandas.pydata.org/docs/reference/api/pandas.DataFrame.duplicated.html>
- <https://stackoverflow.com/a/28256912/1688203>

``` py
tbl = pandas.DataFrame(
    {
        "brand": ["some", "ololo", "some", "another", "ololo", "some"],
        "style": ["cup", "cup", "cup", "thing", "pack", "cup"],
        "rating": [4, 3, 4, 5, 5, 4]
    }
)
print("Original table:")
print(tbl)
#      brand  style  rating
# 0     some    cup       4
# 1    ololo    cup       3
# 2     some    cup       4
# 3  another  thing       5
# 4    ololo   pack       5
# 5     some    cup       4

#tbldubl = numpy.where(tbl.duplicated())[0] # subset=["style"]
tbldubl = tbl.duplicated() # subset=["style"]
tbldubl = tbldubl[tbldubl].index
print(f"Indexes of duplicate rows: {tbldubl}\n")
# Index([2, 5], dtype='int64')

tbldublidx = tbl.index.isin(tbldubl)
print("Table with duplicate rows only:")
print(tbl[tbldublidx])
#   brand style  rating
# 2  some   cup       4
# 5  some   cup       4
print("Table without duplicate rows:")
print(tbl[~tbldublidx])
#      brand  style  rating
# 0     some    cup       4
# 1    ololo    cup       3
# 3  another  thing       5
# 4    ololo   pack       5
```
