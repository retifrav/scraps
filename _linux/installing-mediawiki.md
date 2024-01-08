## MediaWiki

Based on [this tutorial](https://www.howtoforge.com/how-to-install-mediawiki-with-nginx-and-lets-encrypt-ssl-on-ubuntu-20-04/).

<!-- MarkdownTOC -->

- [Preparation](#preparation)
- [MariaDB](#mariadb)
- [PHP](#php)
    - [Increasing request timeout](#increasing-request-timeout)
- [MediaWiki](#mediawiki)

<!-- /MarkdownTOC -->

### Preparation

Usual [new GNU/Linux server](https://github.com/retifrav/scraps/blob/master/_linux/new-linux-server.md) routine (*including NGINX installation*).

### MariaDB

``` sh
$ sudo apt install software-properties-common dirmngr apt-transport-https
$ sudo apt-key adv --fetch-keys 'https://mariadb.org/mariadb_release_signing_key.asc'

$ sudo nano /etc/apt/sources.list.d/mariadb.list
```

Choose a [mirror](https://mariadb.org/download/?t=repo-config):

```
deb [arch=amd64,arm64] https://mirrors.dotsrc.org/mariadb/repo/10.11/ubuntu jammy main
```

``` sh
$ sudo apt update
$ sudo apt install mariadb-server
$ sudo systemctl status mariadb

$ mysql -V
mysql  Ver 15.1 Distrib 10.11.6-MariaDB, for debian-linux-gnu (x86_64) using  EditLine wrapper
```

``` sh
$ sudo mysql_secure_installation
```
```
In order to log into MariaDB to secure it, we'll need the current
password for the root user. If you've just installed MariaDB, and
haven't set the root password yet, you should just press enter here.

Enter current password for root (enter for none):
OK, successfully used password, moving on...

Setting the root password or using the unix_socket ensures that nobody
can log into the MariaDB root user without the proper authorisation.

You already have your root account protected, so you can safely answer 'n'.

Switch to unix_socket authentication [Y/n] n
 ... skipping.

You already have your root account protected, so you can safely answer 'n'.

Change the root password? [Y/n] Y
New password:
Re-enter new password:
Password updated successfully!
Reloading privilege tables..
 ... Success!

By default, a MariaDB installation has an anonymous user, allowing anyone
to log into MariaDB without having to have a user account created for
them.  This is intended only for testing, and to make the installation
go a bit smoother.  You should remove them before moving into a
production environment.

Remove anonymous users? [Y/n] Y
 ... Success!

Normally, root should only be allowed to connect from 'localhost'.  This
ensures that someone cannot guess at the root password from the network.

Disallow root login remotely? [Y/n] Y
 ... Success!

By default, MariaDB comes with a database named 'test' that anyone can
access.  This is also intended only for testing, and should be removed
before moving into a production environment.

Remove test database and access to it? [Y/n] Y
 - Dropping test database...
 ... Success!
 - Removing privileges on test database...
 ... Success!

Reloading the privilege tables will ensure that all changes made so far
will take effect immediately.

Reload privilege tables now? [Y/n] Y
 ... Success!

Cleaning up...

All done!  If you've completed all of the above steps, your MariaDB
installation should now be secure.

Thanks for using MariaDB!
```

``` sh
$ mysql -uroot -p
```
```
Server version: 10.11.6-MariaDB-1:10.11.6+maria~ubu2204 mariadb.org binary distribution

MariaDB [(none)]> CREATE DATABASE mediadb;
Query OK, 1 row affected (0.000 sec)

MariaDB [(none)]> GRANT ALL PRIVILEGES ON mediadb.* TO 'mediauser'@'localhost' IDENTIFIED BY 'SOME-PASSWORD';
Query OK, 0 rows affected (0.003 sec)

MariaDB [(none)]> FLUSH PRIVILEGES;
Query OK, 0 rows affected (0.001 sec)

MariaDB [(none)]> exit;
```

### PHP

``` sh
$ sudo apt install apt-transport-https ca-certificates imagemagick
$ sudo apt install php php-fpm php-intl php-xml php-curl php-gd php-mbstring php-mysql php-apcu

$ php-fpm8.1 --version
PHP 8.1.2-1ubuntu2.14 (fpm-fcgi) (built: Aug 18 2023 11:41:11)
Copyright (c) The PHP Group
Zend Engine v4.1.2, Copyright (c) Zend Technologies
    with Zend OPcache v8.1.2-1ubuntu2.14, Copyright (c), by Zend Technologies

$ sudo nano /etc/php/8.1/fpm/php.ini
```
```
upload_max_filesize = 11M
post_max_size = 11M
memory_limit = 256M
date.timezone = Europe/Moscow
```
``` sh
$ sudo systemctl restart php8.1-fpm.service
```

It will probably also install Apache, so:

``` sh
$ sudo apt remove --purge apache2 apache2-bin apache2-data
```

#### Increasing request timeout

``` sh
$ sudo nano /etc/php/8.1/fpm/pool.d/www.conf
```
``` ini
request_terminate_timeout = 300
```

``` sh
$ php --ini
$ sudo nano /etc/php/8.1/cli/php.ini
```
``` ini
max_execution_time = 300
```

``` sh
$ sudo systemctl restart php8.1-fpm.service
```

Also note `fastcgi_read_timeout 300` in the NGINX config below.

### MediaWiki

``` sh
$ cd /var/www
$ sudo wget https://releases.wikimedia.org/mediawiki/1.41/mediawiki-1.41.0.tar.gz
$ sudo tar -xvf ./mediawiki-1.41.0.tar.gz
$ ls -l
$ sudo rm ./mediawiki-1.41.0.tar.gz
$ sudo mv ./mediawiki-1.41.0 ./mediawiki
$ sudo chown -R www-data:www-data ./mediawiki
```

``` sh
$ sudo nano /etc/nginx/conf.d/default.conf
```
```
# https://www.nginx.com/resources/wiki/start/topics/recipes/mediawiki/
server {
    server_name wiki;

    root /var/www/mediawiki;
    index  index.php;

    client_max_body_size 5m;
    client_body_timeout 60;

    location / {
        try_files $uri $uri/ @rewrite;
    }

    location @rewrite {
        rewrite ^/(.*)$ /index.php?title=$1&$args;
    }

    location ^~ /maintenance/ {
        return 403;
    }

     location /rest.php {
        try_files $uri $uri/ /rest.php?$args;
    }

    location ~ \.php$ {
        include fastcgi_params;
        fastcgi_pass unix:/var/run/php/php8.1-fpm.sock;
        fastcgi_param SCRIPT_FILENAME $request_filename;
        fastcgi_read_timeout 300;
    }

    location ~* \.(js|css|png|jpg|jpeg|gif|ico)$ {
        try_files $uri /index.php;
        expires max;
        log_not_found off;
    }

    location = /_.gif {
        expires max;
        empty_gif;
    }

    location ^~ /cache/ {
        deny all;
    }

    location /dumps {
        root /var/www/mediawiki/local;
        autoindex on;
    }

    # files in these locations are served as static files, not executed
    location ^~ /uploads/ { }
    location ^~ /images/ { }
}
```

``` sh
$ sudo nginx -t

$ grep www-data /etc/group
$ sudo usermod -a -G www-data nginx

$ sudo systemctl restart nginx.service
```

Now you can open your wiki URL and it will start the configuration procedure, which will generate `LocalSettings.php`.
