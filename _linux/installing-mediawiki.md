## MediaWiki

Based on [this tutorial](https://www.howtoforge.com/how-to-install-mediawiki-with-nginx-and-lets-encrypt-ssl-on-ubuntu-20-04/).

<!-- MarkdownTOC -->

- [Preparation](#preparation)
- [NGINX](#nginx)
- [MariaDB](#mariadb)
- [PHP](#php)
- [Composer](#composer)
- [MediaWiki itself](#mediawiki-itself)

<!-- /MarkdownTOC -->

### Preparation

``` sh
$ lsb_release -a
Description: Ubuntu 20.04.4 LTS
Codename: focal
```

Disable SSH passwords (*but first generate and deploy SSH key*):

``` sh
$ sudo nano /etc/ssh/sshd_config
```
```
ChallengeResponseAuthentication no
PasswordAuthentication no
PermitEmptyPasswords no
```

Fuck Snap, as usual:

``` sh
$ sudo systemctl stop snapd
$ sudo systemctl disable snapd
$ sudo apt autoremove --purge snapd gnome-software-plugin-snap
$ sudo systemctl daemon-reload
$ rm -rf ~/snap
$ sudo rm -rf /var/snap /var/cache/snapd /usr/lib/snapd
```

Update the system:

```
$ sudo apt update
$ sudo apt upgrade
```

### NGINX

``` sh
$ sudo apt remove --purge apache2

$ lsb_release -a
$ sudo nano /etc/apt/sources.list.d/nginx.list
```
```
deb https://nginx.org/packages/ubuntu/ focal nginx
deb-src https://nginx.org/packages/ubuntu/ focal nginx
```

``` sh
$ sudo apt update
$ sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys ABF5BD827BD9BF62
$ sudo apt update
$ sudo apt install nginx

$ nginx -version
nginx version: nginx/1.20.2

$ sudo mkdir -p /var/www/wiki-test && cd $_
$ cd ..
$ sudo git clone https://github.com/retifrav/default-web-server-index.git wiki-test
$ ls -l wiki-test/
$ sudo chown -R www-data:www-data /var/www

$ sudo nano /etc/nginx/conf.d/default.conf
root /var/www/wiki-test;

$ sudo systemctl start nginx.service
$ sudo systemctl status nginx.service
```

### MariaDB

``` sh
$ sudo apt-get install software-properties-common dirmngr apt-transport-https
$ sudo apt-key adv --fetch-keys 'https://mariadb.org/mariadb_release_signing_key.asc'

$ sudo nano /etc/apt/sources.list.d/mariadb.list
```

Choose a [mirror](https://mariadb.org/download/?t=repo-config):

```
deb [arch=amd64,arm64,ppc64el,s390x] https://mirrors.dotsrc.org/mariadb/repo/10.7/ubuntu focal main
```

``` sh
$ sudo apt update
$ sudo apt install mariadb-server
$ sudo systemctl status mariadb

$ mysql -V
mysql  Ver 15.1 Distrib 10.7.3-MariaDB, for debian-linux-gnu (x86_64) using readline 5.2
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
Server version: 10.7.3-MariaDB-1:10.7.3+maria~focal mariadb.org binary distribution

MariaDB [(none)]> CREATE DATABASE mediadb;
Query OK, 1 row affected (0.000 sec)

MariaDB [(none)]> GRANT ALL PRIVILEGES ON mediadb.* TO 'mediauser'@'localhost' IDENTIFIED BY 'SOME-PASSWORD';
Query OK, 0 rows affected (0.003 sec)

MariaDB [(none)]> FLUSH PRIVILEGES;
Query OK, 0 rows affected (0.001 sec)

MariaDB [(none)]> exit;
Bye
```

### PHP

MediaWiki 1.37.1 [requires](https://phabricator.wikimedia.org/T248925) PHP 7.4.

``` sh
$ sudo apt install apt-transport-https ca-certificates imagemagick
$ sudo apt install php7.4-fpm php7.4-intl php7.4-xml php7.4-curl php7.4-gd php7.4-mbstring php7.4-mysql php-apcu

$ php-fpm7.4 --version
PHP 7.4.3 (fpm-fcgi) (built: Mar  2 2022 15:36:52)
Copyright (c) The PHP Group
Zend Engine v3.4.0, Copyright (c) Zend Technologies
    with Zend OPcache v7.4.3, Copyright (c), by Zend Technologies

$ sudo nano /etc/php/7.4/fpm/php.ini
```
```
upload_max_filesize = 11M
post_max_size = 11M
memory_limit = 256M
date.timezone = Europe/Moscow
```

``` sh
$ sudo systemctl restart php7.4-fpm.service
```

### Composer

Install Composer using instructions on [its website](https://getcomposer.org/download/) (*the `apt` version is obsolete*):

``` sh
$ php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');"
$ php -r "if (hash_file('sha384', 'composer-setup.php') === '906a84df04cea2aa72f40b5f787e49f22d4c2f19492ac310e8cba5b96ac8b64115ac402c8cd292b8a03482574915d1a8') { echo 'Installer verified'; } else { echo 'Installer corrupt'; unlink('composer-setup.php'); } echo PHP_EOL;"
$ php composer-setup.php
$ php -r "unlink('composer-setup.php');"
$ sudo mv composer.phar /usr/local/bin/composer
```

Latest Composer has problems with MediaWiki (*something about case-sensitive module names or something*), or rather MediaWiki has some issues, so you'll need to downgrade Composer:

``` sh
$ sudo /usr/local/bin/composer self-update 2.1.14

$ /usr/local/bin/composer --version
Composer version 2.1.14 2021-11-30 10:51:43
```

### MediaWiki itself

``` sh
$ cd /var/www
$ sudo wget https://releases.wikimedia.org/mediawiki/1.37/mediawiki-1.37.1.tar.gz
$ sudo tar -xvf ./mediawiki-1.37.1.tar.gz
$ ls -l
$ sudo rm ./mediawiki-1.37.1.tar.gz
$ sudo mv ./mediawiki-1.37.1 ./mediawiki
$ sudo chown -R www-data:www-data ./mediawiki
$ cd ./mediawiki
$ sudo -u www-data /usr/local/bin/composer install --no-dev
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
        fastcgi_pass unix:/var/run/php/php7.4-fpm.sock;
        fastcgi_param SCRIPT_FILENAME $request_filename;
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
