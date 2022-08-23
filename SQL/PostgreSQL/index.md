## PostgreSQL

<!-- MarkdownTOC -->

- [Installation](#installation)
    - [GNU/Linux](#gnulinux)
    - [Windows](#windows)
- [Allow remote connections](#allow-remote-connections)
- [Database](#database)
    - [Add a new database and a user for it](#add-a-new-database-and-a-user-for-it)
    - [Check encoding](#check-encoding)
    - [List tables](#list-tables)
    - [List sequences](#list-sequences)
    - [Backups](#backups)
        - [As a bare SQL](#as-a-bare-sql)
        - [As a binary compressed dump](#as-a-binary-compressed-dump)
    - [Drop database with active connections](#drop-database-with-active-connections)
- [Users](#users)
    - [List users](#list-users)
    - [View users roles](#view-users-roles)
    - [View user grants on tables](#view-user-grants-on-tables)
    - [Create new user with certain rights](#create-new-user-with-certain-rights)
- [Tables](#tables)
    - [Rename a column](#rename-a-column)
    - [Add a column](#add-a-column)
    - [Make a column unique](#make-a-column-unique)
    - [Change default column value](#change-default-column-value)
    - [Create a table with auto-incrementing primary key](#create-a-table-with-auto-incrementing-primary-key)
    - [Add a foreign key](#add-a-foreign-key)

<!-- /MarkdownTOC -->

### Installation

#### GNU/Linux

<https://www.postgresql.org/download/linux/ubuntu/>

``` sh
# create the file repository configuration
$ sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list'

# import the repository signing key
$ wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -

# update the package lists:
$ sudo apt update

# install the latest version of PostgreSQL
$ sudo apt-get install postgresql
```

<https://help.ubuntu.com/community/PostgreSQL#Basic_Server_Setup>

```
$ sudo -u postgres psql postgres

\password postgres
\q
```

Just in case, check that `pg_hba.conf` has `md5` and not `trust`.

#### Windows

Download binaries ZIP archive from <https://www.enterprisedb.com/download-postgresql-binaries>, unpack somewhere (`/d/programs/postgresql/`).

``` sh
$ cd /d/programs/postgresql/bin
$ mkdir -p /d/databases/postgresql
$ ./initdb.exe -D d:/databases/postgresql --username=postgres -E 'UTF-8' --lc-collate='en_US.UTF-8' --lc-ctype='en_US.UTF-8' --lc-messages='en_US.UTF-8' --lc-monetary='en_US.UTF-8' --lc-numeric='en_US.UTF-8' --lc-time='en_US.UTF-8'
$ ./pg_ctl.exe start -D d:/databases/postgresql
```

Connect to the service from a different console:

``` sh
$ ./psql.exe -u postgres
```

And set the password:

```
postgres=# \password postgres
postgres=# \q
```

or:

```
postgres=# ALTER USER postgres WITH ENCRYPTED PASSWORD 'SOME-PASSWORD';
postgres=# \q
```

Then enforce asking for passwords:

``` sh
$ nano d:/databases/postgresql/pg_hba.conf
```

replace `trust` with `md5`:

```
# TYPE  DATABASE        USER            ADDRESS                 METHOD

# "local" is for Unix domain socket connections only
local   all             all                                     md5
# IPv4 local connections:
host    all             all             127.0.0.1/32            md5
# IPv6 local connections:
host    all             all             ::1/128                 md5
# Allow replication connections from localhost, by a user with the
# replication privilege.
local   replication     all                                     md5
host    replication     all             127.0.0.1/32            md5
host    replication     all             ::1/128                 md5
```

And restart the server.

### Allow remote connections

Where the config file is:

```
$ sudo -u postgres psql -c 'SHOW config_file'
               config_file
-----------------------------------------
 /etc/postgresql/12/main/postgresql.conf
(1 row)

$ sudo nano /etc/postgresql/12/main/postgresql.conf

listen_addresses = '*'
```

Where the `pg_hba.conf` is:

```
$ grep hba_file /etc/postgresql/12/main/postgresql.conf
hba_file = '/etc/postgresql/12/main/pg_hba.conf'

$ sudo nano /etc/postgresql/12/main/pg_hba.conf

# connections from local network
host    all             all             192.168.1.0/24          md5
```

### Database

#### Add a new database and a user for it

``` sh
$ sudo -u postgres psql
```
```
postgres=# CREATE DATABASE some_database;
postgres=# CREATE USER some_user WITH ENCRYPTED PASSWORD 'SOME-PASSWORD';
postgres=# GRANT ALL PRIVILEGES ON DATABASE some_database TO some_user;
```

Just in case, database name and user name need to be lower-cased.

#### Check encoding

```
postgres=# \c YOUR-DATABASE
You are now connected to database "YOUR-DATABASE" as user "postgres".
YOUR-DATABASE=# SHOW SERVER_ENCODING;
 server_encoding
-----------------
 UTF8
(1 row)
```

or:

``` sql
SELECT pg_encoding_to_char(encoding) FROM pg_database WHERE datname = 'SOME-DATABASE';

 pg_encoding_to_char
---------------------
 UTF8
(1 row)
```

#### List tables

``` sql
SELECT * FROM information_schema.tables;
```

#### List sequences

``` sql
SELECT * FROM information_schema.sequences;
```

#### Backups

To run `pg_dump` from your scripts, create `~/.pgpass` file:

``` sh
$ nano ~/.pgpass
localhost:5432:*:some_user:some_password

$ chmod 600 ~/.pgpass
```

If your password contains `\` symbols, they need to be escaped as `\\`.

Also for this to work `pg_hba.conf` should have `md5`, not `peer`:

```
local   all             postgres                                md5
local   all             some_user                               md5
```

##### As a bare SQL

Backup:

``` sh
$ pg_dump -U some_user database_name > ./database-backup.sql
```

Restore:

``` sh
$ psql -U some_user -d database_name < ./database-backup.sql
```

##### As a binary compressed dump

Backup:

``` sh
$ pg_dump -Fc -U postgres -f ./database-backup.dump database_name
```

Restore:

``` sh
$ pg_restore -U postgres --clean --dbname database_name ./database-backup.dump
```

If database doesn't exist yet, create it first:

``` sh
$ psql -U postgres
```
```
postgres=# CREATE DATABASE some_database;
postgres=# CREATE USER some_user WITH ENCRYPTED PASSWORD 'SOME-PASSWORD';
postgres=# GRANT ALL PRIVILEGES ON DATABASE some_database TO some_user;
postgres=# \q
```

I don't fucking know why it cannot just create the database if it doesn't exist. You can try to figure it out [here](https://dba.stackexchange.com/a/298972/61935) or [here](https://stackoverflow.com/questions/2732474/restore-a-postgres-backup-file-using-the-command-line).

#### Drop database with active connections

If

``` sql
DROP DATABASE SOME-DATABASE;
```

fails with

```
database SOME-DATABASE is being accessed by other users
```

then

``` sql
SELECT pid, pg_terminate_backend(pid)
FROM pg_stat_activity
WHERE datname = 'SOME-DATABASE' AND pid <> pg_backend_pid();
```

and again

``` sql
DROP DATABASE SOME-DATABASE;
```

### Users

#### List users

```
postgres=# \du
```

or

```
postgres=# \du+
```

#### View users roles

``` sql
SELECT usename AS role_name,
  CASE
     WHEN usesuper AND usecreatedb THEN
       CAST('superuser, create database' AS pg_catalog.text)
     WHEN usesuper THEN
        CAST('superuser' AS pg_catalog.text)
     WHEN usecreatedb THEN
        CAST('create database' AS pg_catalog.text)
     ELSE
        CAST('' AS pg_catalog.text)
  END role_attributes
FROM pg_catalog.pg_user
ORDER BY role_name desc;
```

#### View user grants on tables

``` sql
SELECT *
FROM information_schema.table_privileges
WHERE grantee = 'someuser'
LIMIT 15;
```

#### Create new user with certain rights

``` sql
CREATE USER someuser WITH
    ENCRYPTED PASSWORD 'somepassword'
    NOSUPERUSER
    NOCREATEDB
    NOCREATEROLE
    INHERIT
    NOREPLICATION
    CONNECTION LIMIT -1;

GRANT SELECT,INSERT ON TABLE some_table TO someuser;
GRANT USAGE,SELECT ON SEQUENCE some_id_seq TO someuser;
```

### Tables

#### Rename a column

``` sql
ALTER TABLE hardware RENAME COLUMN owner TO owner_id;
```

#### Add a column

``` sql
ALTER TABLE hardware ADD COLUMN location_id int4 NOT NULL;
```

#### Make a column unique

``` sql
ALTER TABLE hardware ADD UNIQUE (serial_number);
```

#### Change default column value

Just a simple string value:

``` sql
ALTER TABLE ONLY table_name ALTER COLUMN column_name SET DEFAULT 'ololo';
```

Or set it to be today's date(time):

``` sql
ALTER TABLE ONLY table_name ALTER COLUMN some_dt SET DEFAULT now();
```

#### Create a table with auto-incrementing primary key

``` sql
CREATE SEQUENCE locations_id_seq START WITH 0;
CREATE TABLE "locations" (
  "id" int4 NOT NULL DEFAULT nextval('locations_id_seq'::regclass),
  "name" varchar(50) NOT NULL,
  PRIMARY KEY ("id")
);
```

or:

``` sql
CREATE TABLE "locations" (
  "id" SERIAL,
  "name" varchar(50) NOT NULL,
  PRIMARY KEY ("id")
);
```

#### Add a foreign key

``` sql
ALTER TABLE "hardware" ADD CONSTRAINT "hardware_fk1" FOREIGN KEY ("location_id") REFERENCES "locations" ("id") ON DELETE NO ACTION ON UPDATE NO ACTION;
```
