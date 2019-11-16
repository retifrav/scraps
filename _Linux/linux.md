## Linux

- [Versions](#versions)
  - [Linux](#linux)
  - [OpenGL](#opengl)
- [Packages](#packages)
  - [Update packages](#update-packages)
  - [Delete packages](#delete-packages)
- [Users](#users)
  - [All users in the system](#all-users-in-the-system)
  - [Create a new user](#create-a-new-user)
  - [Change your password](#change-your-password)
  - [Last logon](#last-logon)
- [Groups](#groups)
  - [List current user groups](#list-current-user-groups)
  - [List all the groups](#list-all-the-groups)
  - [Create new group](#create-new-group)
  - [Add user to the group](#add-user-to-the-group)
  - [List users of the group](#list-users-of-the-group)
  - [Change owner group of the folder](#change-owner-group-of-the-folder)
- [Renew IP address](#renew-ip-address)
- [CPU temperature](#cpu-temperature)
- [Web-servers](#web-servers)
  - [Get web-server version](#get-web-server-version)
  - [lighttpd](#lighttpd)
  - [Basic authentication](#basic-authentication)
    - [nginx](#nginx)
    - [Apache](#apache)
- [Set up a new server for NET Core deployment](#set-up-a-new-server-for-net-core-deployment)
- [Convert bunch of images](#convert-bunch-of-images)
- [Files and folders](#files-and-folders)
  - [List files](#list-files)
  - [Get the size of a directory](#get-the-size-of-a-directory)
  - [Create a directory and open it](#create-a-directory-and-open-it)
  - [Do something based on directory existence](#do-something-based-on-directory-existence)
  - [Sync folders](#sync-folders)
  - [Replace text in files](#replace-text-in-files)
  - [Find some files and delete them](#find-some-files-and-delete-them)
  - [Preview ZIP archive contents](#preview-zip-archive-contents)
- [Working with FTP](#working-with-ftp)
  - [ftp](#ftp)
  - [lftp](#lftp)
- [Network](#network)
  - [Open ports](#open-ports)
  - [Scan local network](#scan-local-network)
- [SSH](#ssh)
  - [Generate a new SSH key](#generate-a-new-ssh-key)
  - [SSH config example](#ssh-config-example)
  - [Ignore changed remote host identification](#ignore-changed-remote-host-identification)
  - [Disable SSH passwords](#disable-ssh-passwords)
  - [Open a tunnel to some port](#open-a-tunnel-to-some-port)
- [Automount media on startup](#automount-media-on-startup)
- [Build something from source](#build-something-from-source)
- [Get return code](#get-return-code)
- [systemd](#systemd)
  - [Create a new service](#create-a-new-service)
  - [Status of the service](#status-of-the-service)
  - [View log of the service](#view-log-of-the-service)
  - [Restart the service](#restart-the-service)
  - [Reload changed configuration](#reload-changed-configuration)
- [Run commands in background](#run-commands-in-background)
- [GRUB](#grub)
  - [Default boot option](#default-boot-option)
- [Set time zone](#set-time-zone)
- [Cron](#cron)

### Versions

#### Linux

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

#### OpenGL

``` bash
glxinfo | grep "OpenGL version"
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

### Groups

#### List current user groups

```bash
groups
```
#### List all the groups

``` bash
cut -d: -f1 /etc/group | sort
```

#### Create new group

``` bash
groupadd NEW-GROUP
```

#### Add user to the group

``` bash
usermod -a -G NEW-GROUP USERNAME
```

#### List users of the group

``` bash
grep NEW-GROUP /etc/group
```

#### Change owner group of the folder

``` bash
chgrp -R NEW-GROUP /etc/SOME-FOLDER/
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

### Web-servers

#### Get web-server version

``` bash
curl -s -I example.com|awk '$1~/Server:/ {print $2}'
```

#### lighttpd

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

#### Basic authentication

Get password generator:

```
sudo apt install apache2-utils
```

Add a new user/password:

```
sudo htpasswd -c /etc/nginx/.htpasswd someusername
```

And configure your website to use this file for Basic Authentication.

##### nginx

```
location / {
        try_files $uri $uri/ =404;

        auth_basic           "restricted area";
        auth_basic_user_file /etc/nginx/.htpasswd;
}
```

##### Apache

```
<VirtualHost *:8998>
        
        ...

        <Directory "/var/www/website">
                AuthType Basic
                AuthName "Restricted Content"
                AuthUserFile /etc/apache2/.htpasswd
                Require valid-user
        </Directory>

</VirtualHost>
```

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

### Files and folders

#### List files

Table view with all the files and human readable sizes:

``` bash
$ ls -lah

drwxr-xr-x  12 vasya  root   384B Mar  5 20:45 ./
drwxr-xr-x 138 vasya  root   4.3K Mar  5 20:38 ../
-rw-r--r--   1 vasya  root   136K Dec 31  2017 4k-nok-mistake.jpg
-rw-r--r--   1 vasya  root   105K Jan  1  2018 fisherman-friend.jpg
-rw-r--r--   1 vasya  root   188K Jan  1  2018 import-calculator.png
-rw-r--r--   1 vasya  root   134K Jan  1  2018 mobile-internet.png
-rw-r--r--   1 vasya  root    88K Jan  3  2018 nordea-feil.png
-rw-r--r--   1 vasya  root    42K Jan  3  2018 online-payment-fail.png
-rw-r--r--   1 vasya  root    92K Jan  3  2018 ruter-app-fail.png
-rw-r--r--   1 vasya  root   481K Dec 31  2017 ruter-fail.JPG
-rw-r--r--   1 vasya  root   222K Jan  3  2018 ruter-transports.png
-rw-r--r--   1 vasya  root    49K Dec 29  2010 tvoe-litso.jpg
```

Show only the filenames and their sizes:

``` bash
$ ls -lah | awk '{print $9 " | " $5}'

 |
./ | 384B
../ | 4.3K
4k-nok-mistake.jpg | 136K
fisherman-friend.jpg | 105K
import-calculator.png | 188K
mobile-internet.png | 134K
nordea-feil.png | 88K
online-payment-fail.png | 42K
ruter-app-fail.png | 92K
ruter-fail.JPG | 481K
ruter-transports.png | 222K
tvoe-litso.jpg | 49K
```

A `tree` alternative:

```
find . | sed -e "s/[^-][^\/]*\//  |/g" -e "s/|\([^ ]\)/|-\1/"
```

#### Get the size of a directory

``` bash
du -hs /path/to/directory
```

* `-h` - human-readable size
* `-s` - summary, shows the total size only for that directory, otherwise it will show it for all the child ones too

#### Create a directory and open it

``` bash
mkdir ololo && cd "$_"
```
* `$_` - special parameter that holds the last *argument* of the previous command

#### Do something based on directory existence

``` bash
[ -d "somedir" ] && echo "directory exists" || echo "directory does not exist"
```

#### Sync folders

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

#### Replace text in files

``` bash
find ./ -type f -exec sed -i 's/ololo/some\/path/g' {} \;
```

* `find ./` look in the current folder
* `-type f` - apply to files
* `sed -i` - replace all the occurrences of `ololo` string with `some/path` string

#### Find some files and delete them

For example, delete all `.php` files from the folder (and all the subfolders).

```
find . -type f -name "*.php" -exec rm {} +
```

#### Preview ZIP archive contents

```
unzip -l archive.zip | tail -10
```

### Working with FTP

#### ftp

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

#### lftp

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

### Network

#### Open ports

```
netstat -lntup
```

or

```
ss -lntup
```

#### Scan local network

``` bash
nmap -sP 192.168.1.0/24
```

### SSH

#### Generate a new SSH key

``` bash
cd ~/.ssh
ssh-keygen -o -t rsa -b 4096 -C "name@example.org"
```

Leave empty password (or whatever) and set the file name. Change permissions for the files: `chmod 600 id_rsa_newkey*`.

#### SSH config example

An example of a config section (`~/.ssh/config`) for connecting to some remote host:

```
Host mahserver                       # alias for convenience
HostName 216.18.168.16               # actual host address
IdentityFile ~/.ssh/id_rsa_mahserver # SSH key file
User root                            # username
```

#### Ignore changed remote host identification

When you have the same device, and you keep switching Linux installations on it, but DHCP gives it the same IP, so your `~/.ssh/known_hosts` is not happy about it.

``` bash
ssh -o UserKnownHostsFile=/dev/null root@SOME-IP-ADDRESS
```

And that way the "updated" key will not get saved.

#### Disable SSH passwords

So you could connect only by using key.

Your public key should be placed into `~/.ssh/authorized_keys`.

Disable SSH passwords:

``` bash
nano /etc/ssh/sshd_config
```

In this file change `#PasswordAuthentication yes` to `PasswordAuthentication no`.

#### Open a tunnel to some port

``` bash
ssh -N -L 8080:localhost:8080 USERNAME@HOSTNAME
```

And then, for example, all the HTTP requests you send to http://localhost:8080 on your local machine will be actually sent (*tunneled*) to `8080` port of the remote `HOSTNAME`.


### Automount media on startup

``` bash
nano /etc/fstab
```

Suppose, you have NTFS-formated external HDD. Find out its "path" (`/dev/sda1`) and add the following line:

```
/dev/sda1 /media/hdd ntfs-3g defaults 0 0
```

### Build something from source

An example of building `glibc` - because this one is recommended to be installed into a different directory than the default one as it may corrupt the system.

Get sources (either clone or unpack the archive) and then:

``` bash
mkdir build && cd "$_"
../glibc/configure --prefix=/opt/glibc-2.28
make -j4
sudo make install
```

And then you can refer to it with `LD_LIBRARY_PATH=/opt/glibc-2.28/lib/`.

### Get return code

Say, you have some Python script and you want to get its return/exit value:

``` bash
python some.py
RC=$?
echo "Exit code $RC"
```

### systemd

#### Create a new service

Create a config for the new service:

``` bash
nano /etc/systemd/system/some.service
```

Specify the command, environment and user:

``` ini
[Unit]
Description=some

[Service]
WorkingDirectory=/var/www/some/
ExecStart=/usr/bin/dotnet /var/www/some/some.dll
Restart=always
RestartSec=10
SyslogIdentifier=kestrel-some
User=www-data
Environment=ASPNETCORE_ENVIRONMENT=Production

[Install]
WantedBy=multi-user.target
```

Enable and launch it:

``` bash
systemctl enable some.service
systemctl start some.service
```

#### Status of the service

``` bash
systemctl status YOUR-SERVICE.service
```

#### View log of the service

``` bash
journalctl -u YOUR-SERVICE.service
```

Navigation:
* `f` - scroll one page down;
* `g` - scroll to the first line;
* `SHIFT` + `g` - scroll the the last line.

#### Restart the service

``` bash
systemctl restart YOUR-SERVICE.service
```

#### Reload changed configuration

``` bash
systemctl daemon-reload
```

### Run commands in background

Add `&` to the end of the command in order to run it in background::

``` bash
ping ya.ru >> ping.txt &
```

To see the list of running jobs:

``` bash
$ jobs
[1]+  Running                 ping ya.ru >> ping.txt &
```

To stop it by ID:

``` bash
kill %1
```

Or bring it to foreground (also by ID):

``` bash
fg 1
```

And stop it as usual with `CTRL + C`.

### GRUB

#### Default boot option

```
sudo nano etc/default/grub
```

Set the default option, enumeration starts from 0. To be sure, check the list from the boooting menu.

```
sudo update-grub
```

You can also delete unwanted items from `/boot/grub/grub.cfg`.

### Set time zone

```
sudo dpkg-reconfigure tzdata
```

### Cron

```
$ crontab -e

# run at 23:01 every 3 days
1 23 */3 * * /root/backup.sh > /dev/null 2>&1
# run at 01:01 on the 1st of every month
1 1 1 * * certbot renew

$ systemctl restart cron.service
```
