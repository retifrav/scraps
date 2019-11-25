## SQLite

### Basics

Open a database:

```
$ sqlite3 /path/to/some.db
```

All "system" commands start with `.`

List all tables:

```
> .tables
```

Execute a script from file:

```
> .read /tmp/script.sql
```

Exit:

```
> .exit
```

### View table structure

```
> .schema TABLE-NAME
> PRAGMA table_info(TABLE-NAME);
```

### Show column names

```
> .headers ON
> SELECT * FROM TABLE-NAME;
```
