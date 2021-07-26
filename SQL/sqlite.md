## SQLite

<!-- MarkdownTOC -->

- [Open a database](#open-a-database)
- [Exit](#exit)
- [List all tables](#list-all-tables)
- [Execute a script from file](#execute-a-script-from-file)
- [View table structure](#view-table-structure)
- [Show column names](#show-column-names)
- [Get table contents as INSERT](#get-table-contents-as-insert)
- [Modify table](#modify-table)

<!-- /MarkdownTOC -->

### Open a database

```
$ sqlite3 /path/to/some.db
```

### Exit

```
> .exit
```

### List all tables

```
> .tables
```

### Execute a script from file

```
$ sqlite3 /path/to/some.db
> .read /path/to/some.sql
```

or:

``` sh
$ sqlite3 /path/to/some.db < /path/to/some.sql
```

### View table structure

``` sql
> .schema TABLE-NAME
> PRAGMA table_info(TABLE-NAME);
```

### Show column names

``` sql
> .headers ON
> SELECT * FROM TABLE-NAME;
```

### Get table contents as INSERT

``` sql
> .mode
current output mode: list
> .mode insert
> select * from projects;
INSERT INTO "table" VALUES(1,'some',1);
INSERT INTO "table" VALUES(2,'another',0);
INSERT INTO "table" VALUES(3,'ololo',1);
> .mode list
```

### Modify table

SQLite doesn't really support modifying existing tables, so the way to go is to re-create a table (*using transaction*). For example, if you'd like to add a new UNIQUE constraint to the existing field `name`:

``` sql
BEGIN;

ALTER TABLE "projects" RENAME TO "_projects2";

CREATE TABLE "projects" (
    "id" integer NOT NULL,
    "name" text NOT NULL,
    "distributable" integer NOT NULL DEFAULT 1,
    PRIMARY KEY(id),
    UNIQUE(name COLLATE NOCASE) -- that's what's new about this table
);

-- copy the data from the "old" table
INSERT INTO "projects" SELECT * FROM "_projects2";

-- delete the old table
DROP TABLE "_projects2";

COMMIT;
```
