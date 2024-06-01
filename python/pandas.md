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
- [Getting values from indexes and rows](#getting-values-from-indexes-and-rows)
- [pandas.Series to numpy.ndarray](#pandasseries-to-numpyndarray)
- [Find duplicate rows](#find-duplicate-rows)
- [Use duplicated values as index for a new table](#use-duplicated-values-as-index-for-a-new-table)

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

An empty table:

``` py
# specifying dtype's isn't really required(?)
someTable = pandas.DataFrame(
    {
        "a": pandas.Series(dtype="float"),
        "b": pandas.Series(dtype="float"),
        "c": pandas.Series(dtype="str"),
    }#,
    #index=[0]
)
```

A table with some values:

``` py
someTable = pandas.DataFrame(
    {
        "a": [0, 0, 23, 2,  0, 0, 0, 19, 1,  0, 0],
        "b": [0, 0, 2,  3,  0, 0, 0, 4,  3,  0, 0],
        "c": [0, 0, 65, 34, 0, 0, 0, 45, 2,  0, 0],
        "d": [0, 0, 33, 7,  0, 0, 0, 64, 33, 0, 0],
        "e": [0, 0, 0,  45, 0, 0, 0, 12, 11, 0, 0]
    },
    index=[4, 21, 30, 57, 59, 62, 71, 80, 81, 102, 126]
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

### Getting values from indexes and rows

``` py
# https://github.com/retifrav/vulcan-flares/blob/master/atm/stellar_flux/gj876_sflux_timesteps_60sec.pkl
pnd = pandas.read_pickle("/path/to/fluxes.pkl")
print(pnd.shape)
# (1439, 8099)

print(pnd.index.values[0])
# 0.0
print(len(pnd.index))
# 1439

# print(pnd.iloc[0])
# print(pnd.index[1])
# print(pnd.iloc[0].index[-1])

x = pnd.iloc[0].array
print(x[0])  # flux
# 0.8755399806495828
y = pnd.iloc[0].index
print(y[0])  # wave length
# 10.0
```

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
# https://github.com/retifrav/vulcan-flares/blob/master/atm/stellar_flux/gj876_sflux_timesteps_60sec.pkl
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

### Use duplicated values as index for a new table

``` py
import pandas
import numpy

rng = numpy.random.default_rng()
someTable = pandas.DataFrame(
    rng.integers(0, 50, size=(10, 5)),
    #index=[1, 2, 3],
    columns=["a", "b", "c", "d", "e"]
)
display(someTable)

#     a   b   c   d   e
# 0  21  18  27  10  11
# 1  29  36   1  39  28
# 2  22  25  13   5  35
# 3  13  47  19  11  29
# 4  31  37   6  41   1
# 5   6  23  15  42   3
# 6  46  38  33  26   1
# 7  48  25  31  40  32
# 8  39  43  28   2  43
# 9  14  12  13  43   5

uniques, counts = numpy.unique(someTable.values, return_counts=True)
display(uniques, counts)

# array([ 1,  2,  3,  5,  6, 10, 11, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25,
#       26, 27, 28, 29, 31, 32, 33, 35, 36, 37, 38, 39, 40, 41, 42, 43, 46,
#       47, 48], dtype=int64)
# array([3, 1, 1, 2, 2, 1, 2, 1, 3, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2,
#        1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1], dtype=int64)

# find the duplicated values - those which occur more than once (first occurrence also counts)
indexesOfDuplicates = numpy.argwhere(counts > 1)
duplicates = uniques[indexesOfDuplicates]
print(type(duplicates), duplicates.shape)
# <class 'numpy.ndarray'> (11, 1)
print()

# dictionary with the new index and a list of row-column pairs for each value
coordinates = {}

for i in duplicates:
    vl = i.item()
    coordinates[vl] = []
    print(f"cells with value {vl}:")
    vls = numpy.where(someTable == vl)
    #print(vls)
    for v in range(vls[0].size): # number of this value occurences in the table
        row = someTable.index[vls[0][v]]
        column = someTable.columns.values[vls[1][v]]
        print(f"- row: {row}, column: {column}")
        coordinates[vl].append((row, column))
    print()

# cells with value 1:
# - row: 1, column: c
# - row: 4, column: e
# - row: 6, column: e
# 
# cells with value 5:
# - row: 2, column: d
# - row: 9, column: e
# 
# cells with value 6:
# - row: 4, column: c
# - row: 5, column: a
# 
# cells with value 11:
# - row: 0, column: e
# - row: 3, column: d
# 
# cells with value 13:
# - row: 2, column: c
# - row: 3, column: a
# - row: 9, column: c
# 
# cells with value 25:
# - row: 2, column: b
# - row: 7, column: b
# 
# cells with value 28:
# - row: 1, column: e
# - row: 8, column: c
# 
# cells with value 29:
# - row: 1, column: a
# - row: 3, column: e
# 
# cells with value 31:
# - row: 4, column: a
# - row: 7, column: c
# 
# cells with value 39:
# - row: 1, column: d
# - row: 8, column: a
# 
# cells with value 43:
# - row: 8, column: b
# - row: 8, column: e
# - row: 9, column: d

newTable = pandas.DataFrame(
    numpy.nan,
    index=list(duplicates.flatten()),
    columns=list(range(1, numpy.max(counts)+1)) # or `someTable.columns.values`, but that's likely an overkill
)
display(newTable)

#      1   2   3
# 1  NaN NaN NaN
# 5  NaN NaN NaN
# 6  NaN NaN NaN
# 11 NaN NaN NaN
# 13 NaN NaN NaN
# 25 NaN NaN NaN
# 28 NaN NaN NaN
# 29 NaN NaN NaN
# 31 NaN NaN NaN
# 39 NaN NaN NaN
# 43 NaN NaN NaN

for c in coordinates:
    for cc in range(len(coordinates[c])):
        # fill the new table with column names from coordinates dictionary
        newTable.at[c, newTable.columns.values[cc]] = coordinates[c][cc][1] # or `coordinates[c][cc][0]`, if you'd like rows (old index) instead

display(newTable)

#     1  2    3
# 1   c  e    e
# 5   d  e  NaN
# 6   c  a  NaN
# 11  e  d  NaN
# 13  c  a    c
# 25  b  b  NaN
# 28  e  c  NaN
# 29  a  e  NaN
# 31  a  c  NaN
# 39  d  a  NaN
# 43  b  e    d
```
