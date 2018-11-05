## Linux

- [Get Linux version](#get-linux-version)
- [Packages](#packages)
  + [Update packages](#update-packages)
  + [Delete packages](#delete-packages)
- [Users](#users)
  + [All users in the system](#all-users-in-the-system)
  + [Create a new user](#create-a-new-user)
  + [Change your password](#change-your-password)
  + [Last logon](#last-logon)
- [Renew IP address](#renew-ip-address)
- [CPU temperature](#cpu-temperature)
- [Working with lighttpd server](#working-with-lighttpd-server)
- [Set up a new server for NET Core deployment](#set-up-a-new-server-for-net-core-deployment)
- [Convert bunch of images](#convert-bunch-of-images)
- [Files and folders](#files-and-folders)
  + [Get the size of a directory](#get-the-size-of-a-directory)
  + [Create a directory and open it](#create-a-directory-and-open-it)
  + [Sync folders](#sync-folders)
- [Working with FTP](#working-with-ftp)
  + [ftp](#ftp)
  + [lftp](#lftp)
- [Scan local network](#scan-local-network)
- [SSH](#ssh)
  + [Ignore changed remote host identification](#ignore-changed-remote-host-identification)
  + [Disable SSH passwords](#disable-ssh-passwords)
- [Automount media on startup](#automount-media-on-startup)
- [Get the web-server version](#get-the-web-server-version)
- [Build something from source](#build-something-from-source)
- [Get return code](#get-return-code)

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

### Users

#### All users in the system

``` bash
cat /etc/passwd | awk -F ':' '{ print $1 }'
```

#### Create a new user

With `home` directory and change his password.

``` bash
useradd -m vasya
passwd vasya
```

#### Change your password

``` bash
passwd
```

#### Last logon

``` bash
lastlog
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

Install NGINX and edit the config:

``` bash
apt install nginx
nano /etc/nginx/sites-available/default
```

``` nginx
server {
        listen 80;
        listen [::]:80;

        location / {
                proxy_pass http://localhost:5000;
                proxy_http_version 1.1;
                proxy_set_header Upgrade $http_upgrade;
                proxy_set_header Connection keep-alive;
                proxy_set_header Host $host;
                proxy_cache_bypass $http_upgrade;
        }
}
```

``` bash
nginx -s reload
```

Install MySQL:

``` bash
apt install mysql-server
mysql_secure_installation
```

Create new .NET Core Web API project for test:

``` bash
mkdir -p /var/www/test
cd /var/www/test
dotnet new webapi
chown -R www-data:www-data /var/www/
```

Comment `//app.UseHttpsRedirection();` line in `Startup.cs`.

``` bash
dotnet run
```

Open http://YOUR-IP/api/values

### Convert bunch of images

We want to convert files with `main` in name. The purpose of convertion - to reduce the file size by converting if from PNG to JPG.

We have this:

``` bash
whitechapel-detailed.png
whitechapel-main.png
wisdom-of-the-crowd-detailed.png
wisdom-of-the-crowd-main.png
young-pope-detailed.png
young-pope-main.png
```

Install [ImageMagick](https://www.imagemagick.org/script/index.php).

Run:

``` bash
for f in ./*-main.png; do convert -verbose -quality 50 "$f" "${f%.*}-thumb.jpg"; done
```

Result:

``` bash
whitechapel-detailed.png
whitechapel-main-thumb.jpg
whitechapel-main.png
wisdom-of-the-crowd-detailed.png
wisdom-of-the-crowd-main-thumb.jpg
wisdom-of-the-crowd-main.png
young-pope-detailed.png
young-pope-main-thumb.jpg
young-pope-main.png
```

## Files and folders

### Get the size of a directory

``` bash
du -hs /path/to/directory
```

* `-h` - human-readable size
* `-s` - summary, shows the total size only for that directory, otherwise it will show it for all the child ones too

### Create a directory and open it

``` bash
mkdir ololo && cd "$_"
```
* `$_` - special parameter that holds the last *argument* of the previous command

### Sync folders

For example, when you need to restore NGINX config from a backup:

``` bash
$ tree etc/
etc/
`-- nginx
    |-- nginx.conf
    |-- sites-available
    |   |-- default
    |   `-- protvshows
    `-- sites-enabled
        `-- protvshows -> /etc/nginx/sites-available/protvshows

$ mv etc/ /
mv: cannot move 'etc/' to '/etc': Directory not empty

$ rsync -a etc/ /etc/
```

## Working with FTP

### ftp

``` bash
$ cd /path/to/where/you/want/to/download/files/

$ ftp some.server.io
Name (some.server.io:name): YOUR-LOGIN
331 Password required for YOUR-LOGIN
Password: YOUR-PASSWORD
230 User YOUR-LOGIN logged in

ftp> ls
ftp> cd some-folder
ftp> ls
ftp> hash
Hash mark printing on (1024 bytes/hash mark).
ftp> tick
Hash mark printing off.
Tick counter printing on (10240 bytes/tick increment).
ftp> get some-file.mp4
```

### lftp

[lftp](https://en.wikipedia.org/wiki/Lftp) supports [FTPS](https://en.wikipedia.org/wiki/FTPS).

Create a config file:

``` bash
nano ~/.lftprc
```
```
set ftps:initial-prot P
set ftp:ssl-allow true
set ftp:ssl-force true
set ftp:ssl-protect-data false
set ftp:ssl-protect-list true
set ssl:verify-certificate false

open some.server:21
user USERNAME PASSWORD
```

Connect and download some file:

``` bash
$ lftp
lftp USERNAME@some.server:~> ls
drwxr-xr-x   2 USERNAME  USERNAME         6 Apr 21  2016 download
drwxr-x---  25 USERNAME  USERNAME     12288 Sep 21 18:49 files
lftp USERNAME@some.server:/> cd files
lftp USERNAME@some.server:/files> get something.mkv -o /storage/hdd/tv/
`something.mkv' at 231331800 (35%) 10.52M/s eta:38s [Receiving data]
```

## Scan local network

``` bash
nmap -sP 192.168.1.0/24
```

## SSH

### Ignore changed remote host identification

When you have the same device, and you keep switching Linux installations on it, but DHCP gives it the same IP, so your `~/.ssh/known_hosts` is not happy about it.

``` bash
ssh -o UserKnownHostsFile=/dev/null root@SOME-IP-ADDRESS
```

And that way the "updated" key will not get saved.

### Disable SSH passwords

So you could connect only by using key.

Your public key should be placed into `~/.ssh/authorized_keys`.

Disable SSH passwords:

``` bash
nano /etc/ssh/sshd_config
```

In this file change `#PasswordAuthentication yes` to `PasswordAuthentication no`.

## Automount media on startup

``` bash
nano /etc/fstab
```

Suppose, you have NTFS-formated external HDD. Find out its "path" (`/dev/sda1`) and add the following line:

```
/dev/sda1 /media/hdd ntfs-3g defaults 0 0
```

## Get the web-server version

``` bash
curl -s -I example.com|awk '$1~/Server:/ {print $2}'
```

## Build something from source

An example of building `glibc` - because this one is recommended to be installed into a different directory than the default one as it may corrupt the system.

Get sources (either clone or unpack the archive) and then:

``` bash
mkdir build && cd "$_"
../glibc/configure --prefix=/opt/glibc-2.28
make -j4
sudo make install
```

And then you can refer to it with `LD_LIBRARY_PATH=/opt/glibc-2.28/lib/`.

## Get return code

Say, you have some Python script and you want to get its return/exit value:

``` bash
python some.py
RC=$?
echo "Exit code $RC"
```
