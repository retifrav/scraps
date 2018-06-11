## Linux

- [Get Linux version](#get-linux-version)
- [Packages](#packages)
  + [Update packages](#update-packages)
  + [Delete packages](#delete-packages)
- [Renew IP address](#renew-ip-address)
- [CPU temperature](#cpu-temperature)
- [Working with lighttpd server](#working-with-lighttpd-server)
- [Set up a new server for NET Core deployment](#set-up-a-new-server-for-net-core-deployment)

### Get Linux version

Best way:

``` bash
lsb_release -a
```

Good way:

``` bash
cat /etc/*-release
```

Kernel and stuff:

``` bash
uname -a
```

More stuff:

``` bash
cat /proc/version
```

### Packages

#### Update packages

``` bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade
sudo apt-get autoremove
```

#### Delete packages

For example, we want to delete **LibreOffice**. All of its packages names start with `libreoffice`, so:

``` bash
sudo apt-get remove --purge libreoffice*
sudo apt-get clean
sudo apt-get autoremove
```
### Renew IP address

For example, if host machine has changed the network, and you need to update the IP address in your guest VM:

``` bash
dhclient -v -r
```

### CPU temperature

``` bash
cat /sys/class/thermal/thermal_zone*/temp
```

### Working with lighttpd server

``` bash
sudo mkdir /var/www/some/
sudo echo "ololo" > /var/www/some/some.txt

nano ~/lighttpd.conf
```

``` php
server.document-root = "/var/www/some/"

server.port = 8080

mimetype.assign = (
  ".html" => "text/html",
  ".qml" => "text/plain",
  ".txt" => "text/plain",
  ".jpg" => "image/jpeg",
  ".png" => "image/png"
)
```

``` bash
lighttpd -f ~/lighttpd.conf -D
```

* `-f` - path to the config file
* `-D` - do not run in background

http://localhost:8080/some.txt

### Set up a new server for NET Core deployment

Install .NET Core: https://www.microsoft.com/net/download/linux-package-manager/ubuntu16-04/sdk-current

Install NGINX and create a directory for your website:

``` bash
apt install nginx
mkdir -p /var/www/YOUR-WEBSITE
```

Install MySQL:

``` bash
apt install mysql-server
mysql_secure_installation
```
