### Get a list of all users

``` sql
SELECT User FROM mysql.user;
```

### Create a new user and grant him rights

``` sql
CREATE USER 'NEW-USER'@'localhost' IDENTIFIED BY 'PASSWORD';
GRANT ALL ON DATABASE-NAME.* TO 'NEW-USER'@'localhost';
```

### Get the charset of database

``` sql
SELECT default_character_set_name FROM information_schema.SCHEMATA WHERE schema_name = "YOUR-DATABASE-NAME";
```

```
+---------------------------------+
| default_character_set_name |
+---------------------------------+
| latin1                                |
+---------------------------------+
```

### Change databases's charset to UTF

``` sql
ALTER DATABASE your-database-name CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

And you will also need to change it for existing tables:

``` sql
ALTER TABLE your-table-name CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

UTF-8 is a variable-length encoding. In the case of UTF-8, this means that storing one code point requires one to four bytes. However, MySQL's encoding called "utf8" only stores a maximum of three bytes per code point.

So if you want your column to support storing characters lying outside the BMP (and you usually want to), such as emoji, use "utf8mb4".

### Backup and restore database

Save the `.sql` dump in your home folder:

``` cmd
mysqldump -u root -p DATABASE-NAME -r ~/backup.sql
```

And restore a database from it:

``` cmd
mysql -u root -p
```

``` sql
drop database DATABASE-NAME;
create database DATABASE-NAME;
GRANT ALL ON DATABASE-NAME.* TO 'someuser'@'localhost';
SOURCE ~/backup.sql
```
