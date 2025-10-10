## MySQL

MySQL and MariaDB.

<!-- MarkdownTOC -->

- [Configuration and maintenance](#configuration-and-maintenance)
    - [System variables](#system-variables)
    - [Find config file](#find-config-file)
    - [Limit RAM usage](#limit-ram-usage)
    - [Last executed queries](#last-executed-queries)
        - [Via SQL](#via-sql)
        - [Via config](#via-config)
    - [Reset root password](#reset-root-password)
- [Database](#database)
    - [Create a new database with specific charset](#create-a-new-database-with-specific-charset)
    - [Backup and restore database](#backup-and-restore-database)
    - [Get the charset of database](#get-the-charset-of-database)
    - [Change databases charset to UTF](#change-databases-charset-to-utf)
    - [Get database engine](#get-database-engine)
- [Users](#users)
    - [Get a list of all users](#get-a-list-of-all-users)
    - [Create a new user and grant him rights](#create-a-new-user-and-grant-him-rights)
    - [Grant user rights for procedures](#grant-user-rights-for-procedures)
    - [Delete user](#delete-user)
    - [View privileges](#view-privileges)
- [Tables](#tables)
    - [Get information about the table](#get-information-about-the-table)
    - [Add a new column](#add-a-new-column)
    - [Delete a column](#delete-a-column)
    - [Change a column](#change-a-column)
    - [Add or delete a foreign key](#add-or-delete-a-foreign-key)
- [Stored procedures](#stored-procedures)
    - [List of all stored procedures](#list-of-all-stored-procedures)
    - [Show stored procedure script](#show-stored-procedure-script)
    - [Create a cursor and fill temporary table using it](#create-a-cursor-and-fill-temporary-table-using-it)

<!-- /MarkdownTOC -->

### Configuration and maintenance

#### System variables

``` sql
SHOW VARIABLES;
```

#### Find config file

``` sh
$ mysql --verbose --help | grep my.cnf
                      order of preference, my.cnf, $MYSQL_TCP_PORT,
/etc/my.cnf /etc/mysql/my.cnf /usr/local/mysql/etc/my.cnf ~/.my.cnf
```

#### Limit RAM usage

For InnoDB engine:

``` sh
$ sudo nano /etc/mysql/my.cnf
```
``` ini
[mysqld]
innodb_buffer_pool_size=256M
```

Also consider [enabling swap](/_linux/index.md#swap-and-cache).

#### Last executed queries

##### Via SQL

Enable logging:

``` sql
SET GLOBAL log_output = 'TABLE';
Execute SET GLOBAL general_log = 'ON';
```

And then you'll last queries in this table:

``` sql
SELECT event_time, argument FROM mysql.general_log ORDER BY event_time DESC LIMIT 10;
```

##### Via config

On Mac OS:

``` sh
$ nano ~/.my.cnf

[mysqld]
# enable log
general_log = 1
# log failed/incorrect queries too
log-raw = 1
general_log_file = /Users/YOUR-NAME/dbs/mysql/queries.log
#log_error = /Users/YOUR-NAME/dbs/mysql/error.log
```

#### Reset root password

<https://digitalocean.com/community/tutorials/how-to-reset-your-mysql-or-mariadb-root-password#step-4-changing-the-root-password>

``` sh
$ /path/to/mariadbd-safe --skip-grant-tables --skip-networking
```
``` sql
FLUSH PRIVILEGES;
ALTER USER 'root'@'localhost' IDENTIFIED BY 'YOUR-NEW-PASSWORD-HERE';
exit
```
``` sh
$ mariadb -u root -p
```

### Database

#### Create a new database with specific charset

``` sql
CREATE DATABASE DATABASE-NAME CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

#### Backup and restore database

Dump the database into a `.sql` file:

``` sh
$ mysqldump -u root -p database-name --routines --no-tablespaces -r ./backup.sql
```

To compress it:

``` sh
$ mysqldump -u root -p database-name --routines --no-tablespaces | gzip > ./database.gz
```

You can also add `--no-data`, if you need only schema without data no data (*such as tables contents*).

Now, to restore the database from this backup on another host:

``` cmd
$ mysql -u root -p
```

``` sql
DROP DATABASE database-name;
CREATE DATABASE database-name CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
GRANT ALL PRIVILEGES ON database-name.* TO 'someuser'@'localhost';
FLUSH PRIVILEGES;
USE database-name;
SOURCE ~/backup.sql;
```

The collation/encoding parameter might not be needed.

#### Get the charset of database

``` sql
SELECT default_character_set_name FROM information_schema.SCHEMATA WHERE schema_name = "YOUR-DATABASE-NAME";
```

```
+---------------------------------+
| default_character_set_name      |
+---------------------------------+
| latin1                          |
+---------------------------------+
```

#### Change databases charset to UTF

``` sql
ALTER DATABASE your-database-name CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

And you will also need to change it for existing tables:

``` sql
ALTER TABLE your-table-name CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

UTF-8 is a variable-length encoding. In the case of UTF-8, this means that storing one code point requires one to four bytes. However, MySQL's encoding called "utf8" only stores a maximum of three bytes per code point.

So if you want your column to support storing characters lying outside the BMP (and you usually want to), such as emoji, use "utf8mb4".

#### Get database engine

Default engine:

``` sql
show engines;
+--------------------+---------+-------------------------------------------------------------------------------------------------+--------------+------+------------+
| Engine             | Support | Comment                                                                                         | Transactions | XA   | Savepoints |
+--------------------+---------+-------------------------------------------------------------------------------------------------+--------------+------+------------+
| CSV                | YES     | Stores tables as CSV files                                                                      | NO           | NO   | NO         |
| MRG_MyISAM         | YES     | Collection of identical MyISAM tables                                                           | NO           | NO   | NO         |
| MEMORY             | YES     | Hash based, stored in memory, useful for temporary tables                                       | NO           | NO   | NO         |
| Aria               | YES     | Crash-safe tables with MyISAM heritage. Used for internal temporary tables and privilege tables | NO           | NO   | NO         |
| MyISAM             | YES     | Non-transactional engine with good performance and small data footprint                         | NO           | NO   | NO         |
| SEQUENCE           | YES     | Generated tables filled with sequential values                                                  | YES          | NO   | YES        |
| InnoDB             | DEFAULT | Supports transactions, row-level locking, foreign keys and encryption for tables                | YES          | YES  | YES        |
| PERFORMANCE_SCHEMA | YES     | Performance Schema                                                                              | NO           | NO   | NO         |
+--------------------+---------+-------------------------------------------------------------------------------------------------+--------------+------+------------+
```

Engines per table in a database:

``` sql
SELECT table_name, table_schema, engine FROM information_schema.tables WHERE table_schema='some_database';
```
``` sql
+------------------------+---------------+--------+
| table_name             | table_schema  | engine |
+------------------------+---------------+--------+
| comment                | some_database | InnoDB |
| slot_roles             | some_database | InnoDB |
| trackbacks             | some_database | InnoDB |
...
```

### Users

#### Get a list of all users

``` sql
SELECT User FROM mysql.user;
```

#### Create a new user and grant him rights

``` sql
CREATE USER 'USERNAME'@'localhost' IDENTIFIED BY 'SOME-PASSWORD-HERE';
GRANT ALL PRIVILEGES ON DATABASE-NAME.* to 'USERNAME'@'localhost';
FLUSH PRIVILEGES;
SHOW GRANTS FOR 'USERNAME'@'localhost';
```
``` sql
+---------------------------------------------------------------------------------------------------------------+
| Grants for USERNAME@localhost                                                                                   |
+---------------------------------------------------------------------------------------------------------------+
| GRANT USAGE ON *.* TO `USERNAME`@`localhost` IDENTIFIED BY PASSWORD '*SOME-PASSWORD-HASH-HERE' |
| GRANT ALL PRIVILEGES ON `DATABASE-NAME`.* TO `USERNAME`@`localhost`                                                |
+---------------------------------------------------------------------------------------------------------------+
2 rows in set (0.000 sec)
```

If the database is running in a Docker container and it is expected that other containers in that Docker network will be accessing it, then instead of `localhost` you will need to set that particular Docker network address range (*by using a wildcard*), such as `172.18.%`.

#### Grant user rights for procedures

So he could use `mysqldump`, for example.

``` sql
GRANT SELECT ON mysql.proc to 'USERNAME'@'localhost';
```

#### Delete user

``` sql
DROP USER 'USERNAME'@'localhost';
```

#### View privileges

For current user:

``` sql
SHOW GRANTS;
```

For specific user:

``` sql
SHOW GRANTS FOR 'USERNAME'@'localhost';
```
``` sql
+--------------------------------------------------------------------+
| Grants for USERNAME@localhost                                      |
+--------------------------------------------------------------------+
| GRANT USAGE ON *.* TO 'USERNAME'@'localhost'                       |
| GRANT ALL PRIVILEGES ON `DATABASE`.* TO 'USERNAME'@'localhost'     |
| GRANT SELECT ON `mysql`.`proc` TO 'USERNAME'@'localhost'           |
+--------------------------------------------------------------------+
3 rows in set (0.00 sec)
```

### Tables

#### Get information about the table

`CREATE TABLE` script:

``` sql
SHOW CREATE TABLE table-name;
```

Table structure:

``` sql
DESCRIBE table-name;
```

#### Add a new column

``` sql
ALTER TABLE table-name ADD new-column-name VARCHAR(50) NOT NULL DEFAULT 'default value' AFTER some-existing-column;
```

#### Delete a column

``` sql
ALTER TABLE table-name DROP COLUMN column-name;
```

#### Change a column

Rename:

``` sql
ALTER TABLE table-name CHANGE `old-name` `new-name` VARCHAR(500);
```

Change the type:

``` sql
ALTER TABLE table-name MODIFY column-name BIGINT UNSIGNED NOT NULL;
```

Remove default value:

``` sql
ALTER TABLE table-name ALTER COLUMN column-name DROP DEFAULT;
```

Make it unique:

``` sql
ALTER TABLE table-name ADD UNIQUE (column-name);
```

#### Add or delete a foreign key

Add:

``` sql
ALTER TABLE this-table ADD CONSTRAINT `this-other` FOREIGN KEY (`column-from-this-table`) REFERENCES `other-table` (`column-from-other-table`);
```

Delete:

``` sql
ALTER TABLE table-name DROP FOREIGN KEY key-name;
```

### Stored procedures

#### List of all stored procedures

...and functions for the current database:

```sql
SELECT name, type FROM mysql.proc WHERE db = database();
```

In later versions:

``` sql
SHOW PROCEDURE STATUS WHERE Db = DATABASE() AND Type = 'PROCEDURE';
```

#### Show stored procedure script

``` sql
SHOW CREATE PROCEDURE your-database-name.procedure-name;
```

#### Create a cursor and fill temporary table using it

``` sql
DELIMITER ;;
CREATE PROCEDURE procedure-name()
BEGIN
  DROP TEMPORARY TABLE IF EXISTS tmp;
  CREATE TEMPORARY TABLE tmp (id BIGINT);

  BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE someID BIGINT;

    DECLARE crsr CURSOR FOR SELECT id FROM some-table;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN crsr;

    read_loop: LOOP
      FETCH crsr INTO someID;
      INSERT INTO tmp VALUES(someID);
      IF done THEN
        LEAVE read_loop;
      END IF;
    END LOOP;

    CLOSE crsr;
  END;

  SELECT * FROM tmp;
  DROP TEMPORARY TABLE tmp;
END;;
DELIMITER ;
```

Important detail here is to have a nested `BEGIN`/`END`, otherwise it complains about creating temporary table statement for some reasons.
