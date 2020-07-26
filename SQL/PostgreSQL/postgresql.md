## PostgreSQL

- [Installation](#installation)
- [Allow remote connections](#allow-remote-connections)
- [Add new user and database](#add-new-user-and-database)
- [Check encoding](#check-encoding)

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
postgres=# SHOW SERVER_ENCODING;
 server_encoding
-----------------
 UTF8
(1 row)

postgres=# \c YOUR-DATABASE
You are now connected to database "YOUR-DATABASE" as user "postgres".
YOUR-DATABASE=# SHOW SERVER_ENCODING;
 server_encoding
-----------------
 UTF8
(1 row)
```
