## PostgreSQL

- [Installation](#installation)
- [Allow remote connections](#allow-remote-connections)
- [Add new user and database](#add-new-user-and-database)
- [Check encoding](#check-encoding)
- [Drop database with active connections](#drop-database-with-active-connections)
- [List users](#list-users)
- [Working with tables](#working-with-tables)
  - [Rename a column](#rename-a-column)
  - [Add a column](#add-a-column)
  - [Make a column unique](#make-a-column-unique)
  - [Create a table with auto-incrementing primary key](#create-a-table-with-auto-incrementing-primary-key)
  - [Add a foreign key](#add-a-foreign-key)

### Installation

https://www.postgresql.org/download/linux/ubuntu/

```
# create the file repository configuration
$ sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list'

# import the repository signing key
$ wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -

# update the package lists:
$ sudo apt update

# install the latest version of PostgreSQL
$ sudo apt-get install postgresql
```

https://help.ubuntu.com/community/PostgreSQL#Basic_Server_Setup

```
$ sudo -u postgres psql postgres

\password postgres
\q
```

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

### Add new user and database

```
$ sudo -u postgres psql
postgres=# create database SOME-DATABASE;
postgres=# create user SOME-USER with encrypted password 'SOME-PASSWORD';
postgres=# grant all privileges on database SOME-DATABASE to SOME-USER;
```

### Check encoding

```
postgres=# \c YOUR-DATABASE
You are now connected to database "YOUR-DATABASE" as user "postgres".
YOUR-DATABASE=# SHOW SERVER_ENCODING;
 server_encoding
-----------------
 UTF8
(1 row)
```

or

``` sql
teamcity=# SELECT pg_encoding_to_char(encoding) FROM pg_database WHERE datname = 'SOME-DATABASE';
 pg_encoding_to_char
---------------------
 UTF8
(1 row)
```

### Drop database with active connections

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

### List users

```
postgres=# \du
```

or

```
postgres=# \du+
```

### Working with tables

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
