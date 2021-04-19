## SQLite

<!-- MarkdownTOC -->

- [Basics](#basics)
- [View table structure](#view-table-structure)
- [Show column names](#show-column-names)
- [Get table contents as INSERT](#get-table-contents-as-insert)
- [Modify table](#modify-table)

<!-- /MarkdownTOC -->

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

SQLite doesn't really support modifying existing tables, so the way to go is to re-create a table. For example, if you'd like to add a new UNIQUE constraint:

``` sql
CREATE TABLE "projects2" (
    "id" integer NOT NULL,
    "name" text NOT NULL,
    "distributable" integer NOT NULL DEFAULT 1,
    PRIMARY KEY(id),
    UNIQUE(name COLLATE NOCASE)
);
INSERT INTO projects2 SELECT * FROM projects;
-- make sure that everything was inserted without errors,
-- and only then drop the original table
DROP TABLE projects;
ALTER TABLE projects2 RENAME TO projects;
```
