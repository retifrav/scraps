## SQLite

<!-- MarkdownTOC -->

- [Open a database](#open-a-database)
- [List all tables](#list-all-tables)
- [Execute a script from file](#execute-a-script-from-file)
- [View table structure](#view-table-structure)
- [Turn on showing column names](#turn-on-showing-column-names)
- [Get table contents as INSERT](#get-table-contents-as-insert)
- [Modify table](#modify-table)
- [Backups](#backups)
    - [Make a backup](#make-a-backup)
    - [Restore from a backup](#restore-from-a-backup)

<!-- /MarkdownTOC -->

### Open a database

``` sh
$ sqlite3 /path/to/some.db
```

and exit:

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

Via CLI tool:

``` sql
> .schema TABLE-NAME
> PRAGMA table_info(TABLE-NAME);
```

Or via SQL:

``` sql
SELECT sql 
FROM sqlite_schema 
WHERE name = 'TABLE-NAME';
```

### Turn on showing column names

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

-- if you have foreign keys in other tables referring to this table, then they will all point
-- to `_projects2` now, so you'll need to do a little trick with forth-and-back renaming again,
-- and then they all will get updated
ALTER TABLE "projects" RENAME TO "_projects2";
ALTER TABLE "_projects2" RENAME TO "projects";

COMMIT;
```

### Backups

#### Make a backup

``` sh
$ sqlite3 /path/to/some.db .schema > /path/to/schema.sql
$ sqlite3 /path/to/some.db .dump > /path/to/dump.sql
```

#### Restore from a backup

Same as [executing a script from file](#execute-a-script-from-file):

``` sh
$ sqlite3 /path/to/some.db < /path/to/dump.sql
```

or:

``` sh
$ sqlite3 /path/to/some.db
sqlite> .read /path/to/dump.sql
```
