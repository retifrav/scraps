## Linux

- [Versions](#versions)
  - [Linux](#linux)
  - [OpenGL](#opengl)
- [Packages](#packages)
  - [Update packages](#update-packages)
  - [Delete packages](#delete-packages)
- [Upgrading the system](#upgrading-the-system)
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
  - [NGINX](#nginx)
    - [Logs rotation](#logs-rotation)
  - [lighttpd](#lighttpd)
  - [Basic authentication](#basic-authentication)
    - [NGINX](#nginx-1)
    - [Apache](#apache)
- [Set up a new server for NET Core deployment](#set-up-a-new-server-for-net-core-deployment)
- [Convert bunch of images](#convert-bunch-of-images)
- [Files and folders](#files-and-folders)
  - [List files](#list-files)
  - [Get the size of a directory](#get-the-size-of-a-directory)
  - [Create a directory and open it](#create-a-directory-and-open-it)
  - [Do something based on directory existence](#do-something-based-on-directory-existence)
  - [Sync folders](#sync-folders)
  - [Find some files and delete them](#find-some-files-and-delete-them)
  - [Preview ZIP archive contents](#preview-zip-archive-contents)
  - [Count folders](#count-folders)
  - [Get chmod numerical value](#get-chmod-numerical-value)
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
  - [Enable cron log](#enable-cron-log)
- [sed](#sed)
  - [Delete lines from file](#delete-lines-from-file)
  - [Replace text in files](#replace-text-in-files)
- [Screen](#screen)
- [x509 certificate](#x509-certificate)
- [Allow program to bind to 80 port](#allow-program-to-bind-to-80-port)
- [Define a variable using configure](#define-a-variable-using-configure)
- [iptables](#iptables)

### Versions

#### Linux

Best way:

```
lsb_release -a
```

Good way:

```
cat /etc/*-release
```

Kernel and stuff:

```
uname -a
```

More stuff:

```
cat /proc/version
```

#### OpenGL

```
glxinfo | grep "OpenGL version"
```

### Packages

#### Update packages

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade
sudo apt-get autoremove
```

#### Delete packages

For example, we want to delete **LibreOffice**. All of its packages names start with `libreoffice`, so:

```
sudo apt-get remove --purge libreoffice*
sudo apt-get clean
sudo apt-get autoremove
```

Or, more civilized way, let's remove **minidlna** and its dependencies:

```
sudo apt remove --auto-remove minidlna
```

### Upgrading the system

```
$ sudo apt install update-manager-core
```

Switch from `lts` to `normal` in `Prompt`:

```
$ sudo nano /etc/update-manager/release-upgrades
```

Start a [screen](#screen) session and run:

```
do-release-upgrade
```

### Users

#### All users in the system

```
cat /etc/passwd | awk -F ':' '{ print $1 }'
```

#### Create a new user

With `home` directory and change his password.

```
useradd -m vasya
passwd vasya
```

#### Change your password

```
passwd
```

#### Last logon

```
lastlog
```

### Groups

#### List current user groups

```
groups
```
#### List all the groups

```
cut -d: -f1 /etc/group | sort
```

#### Create new group

```
groupadd NEW-GROUP
```

#### Add user to the group

```
usermod -a -G NEW-GROUP USERNAME
```

#### List users of the group

```
grep NEW-GROUP /etc/group
```

#### Change owner group of the folder

```
chgrp -R NEW-GROUP /etc/SOME-FOLDER/
```

### Renew IP address

For example, if host machine has changed the network, and you need to update the IP address in your guest VM:

```
dhclient -v -r
```

### CPU temperature

```
cat /sys/class/thermal/thermal_zone*/temp
```

### Web-servers

#### Get web-server version

```
curl -s -I example.com|awk '$1~/Server:/ {print $2}'
```

#### NGINX

##### Logs rotation

Log files are split every week and rotated every 8 weeks (2 months).

```
$ sudo nano /etc/logrotate.d/nginx

/var/log/nginx/*.log {
        weekly
        missingok
        rotate 8
        compress
        delaycompress
        notifempty
        create 0640 www-data adm
        sharedscripts
        prerotate
                if [ -d /etc/logrotate.d/httpd-prerotate ]; then \
                        run-parts /etc/logrotate.d/httpd-prerotate; \
                fi \
        endscript
        postrotate
                invoke-rc.d nginx rotate >/dev/null 2>&1
        endscript
}

$ kill -USR1 $(cat /var/run/nginx.pid)
```

#### lighttpd

```
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

```
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

##### NGINX

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

```
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

```
nginx -s reload
```

Install MySQL:

```
apt install mysql-server
mysql_secure_installation
```

Check `root` user authentication and set the password if it's not set:

``` sql
SELECT host, user, authentication_string FROM mysql.user;
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'YOUR-PASSWORD';
```

Create new .NET Core Web API project for test:

```
mkdir -p /var/www/test
cd /var/www/test
dotnet new webapi
chown -R www-data:www-data /var/www/
```

Comment `//app.UseHttpsRedirection();` line in `Startup.cs`.

```
dotnet run
```

Open http://YOUR-IP/api/values

### Convert bunch of images

We want to convert files with `main` in name. The purpose of convertion - to reduce the file size by converting if from PNG to JPG.

We have this:

```
whitechapel-detailed.png
whitechapel-main.png
wisdom-of-the-crowd-detailed.png
wisdom-of-the-crowd-main.png
young-pope-detailed.png
young-pope-main.png
```

Install [ImageMagick](https://www.imagemagick.org/script/index.php).

Run:

```
for f in ./*-main.png; do convert -verbose -quality 50 "$f" "${f%.*}-thumb.jpg"; done
```

Result:

```
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

```
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

```
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

List only files names:

```
ls -l1
```

Reverse order:

```
ls -l1r
```

#### Get the size of a directory

```
du -hs /path/to/directory
```

* `-h` - human-readable size
* `-s` - summary, shows the total size only for that directory, otherwise it will show it for all the child ones too

#### Create a directory and open it

```
mkdir ololo && cd "$_"
```
* `$_` - special parameter that holds the last *argument* of the previous command

#### Do something based on directory existence

```
[ -d "somedir" ] && echo "directory exists" || echo "directory does not exist"
```

#### Sync folders

For example, when you need to restore NGINX config from a backup:

```
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

#### Find some files and delete them

For example, delete all `.php` files from the folder (and all the subfolders).

```
find . -type f -name "*.php" -exec rm {} +
```

#### Preview ZIP archive contents

```
$ less archive.zip
```

or

```
$ unzip -l archive.zip | tail -10
```

#### Count folders

On the current level only:

```
find . -mindepth 1 -maxdepth 1 -type d | wc -l
```

#### Get chmod numerical value

```
stat --format '%a' ~/.ssh/config
```

### Working with FTP

#### ftp

```
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

```
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

```
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

```
nmap -sP 192.168.1.0/24
```

### SSH

#### Generate a new SSH key

```
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

```
ssh -o UserKnownHostsFile=/dev/null root@SOME-IP-ADDRESS
```

And that way the "updated" key will not get saved.

#### Disable SSH passwords

So you could connect only by using key.

Your public key should be placed into `~/.ssh/authorized_keys`.

Disable SSH passwords:

```
nano /etc/ssh/sshd_config
```

In this file change `#PasswordAuthentication yes` to `PasswordAuthentication no`.

#### Open a tunnel to some port

```
ssh -N -L 8080:localhost:8080 USERNAME@HOSTNAME
```

And then, for example, all the HTTP requests you send to http://localhost:8080 on your local machine will be actually sent (*tunneled*) to `8080` port of the remote `HOSTNAME`.


### Automount media on startup

Suppose, you have NTFS-formated external HDD. Find out its "path" (`/dev/sda1`) and:

```
$ sudo nano /etc/fstab

/dev/sda1 /media/hdd ntfs-3g defaults 0 0
```

But media can be discovered with different paths from time to time, so it's more reliable to use UUID or labels:

```
$ sudo blkid

/dev/mmcblk0p1: LABEL_FATBOOT="boot" LABEL="boot" UUID="5203-DF71" TYPE="vfat" PARTUUID="6c526e13-04"
/dev/mmcblk0p2: LABEL="rootfs" UUID="2ab3f8e1-7dc6-41f5-b0db-dd5959d54d4e" TYPE="ext4" PARTUUID="6c586e13-02"
/dev/sda1: LABEL="some" UUID="581e681f-9d3c-4945-b459-eb5086d3002b" TYPE="ext4" PARTUUID="6664625e-01"
/dev/sdb1: LABEL="another" UUID="34E9-3319" TYPE="exfat" PARTUUID="bf87c135-03"
/dev/mmcblk0: PTUUID="6c596e14" PTTYPE="dos"
```

We need USB drives with labels `some` and `another`, but the latter has `exfat` filesystem, so add its support first:

```
sudo apt install exfat-fuse exfat-utils
```

And then:

```
$ sudo mkdir /media/some
$ sudo mkdir /media/another
$ sudo nano /etc/fstab

LABEL=some /media/some ext4 defaults,nofail 0 0
LABEL=another /media/another exfat defaults,nofail 0 0

$ sudo mount -a
```

### Build something from source

An example of building `glibc` - because this one is recommended to be installed into a different directory than the default one as it may corrupt the system.

Get sources (either clone or unpack the archive) and then:

```
mkdir build && cd "$_"
../glibc/configure --prefix=/opt/glibc-2.28
make -j4
sudo make install
```

And then you can refer to it with `LD_LIBRARY_PATH=/opt/glibc-2.28/lib/`.

### Get return code

Say, you have some Python script and you want to get its return/exit value:

```
python some.py
RC=$?
echo "Exit code $RC"
```

### systemd

#### Create a new service

Create a config for the new service:

```
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

```
systemctl enable some.service
systemctl start some.service
```

#### Status of the service

```
systemctl status YOUR-SERVICE.service
```

#### View log of the service

```
journalctl -u YOUR-SERVICE.service
```

Navigation:
* `f` - scroll one page down;
* `g` - scroll to the first line;
* `SHIFT` + `g` - scroll the the last line.

#### Restart the service

```
systemctl restart YOUR-SERVICE.service
```

#### Reload changed configuration

```
systemctl daemon-reload
```

### Run commands in background

Add `&` to the end of the command in order to run it in background::

```
ping ya.ru >> ping.txt &
```

To see the list of running jobs:

```
$ jobs
[1]+  Running                 ping ya.ru >> ping.txt &
```

To stop it by ID:

```
kill %1
```

Or bring it to foreground (also by ID):

```
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

#### Enable cron log

```
$ sudo nano /etc/rsyslog.d/50-default.conf

# uncomment
cron.*                          /var/log/cron.log

$ sudo systemctl restart rsyslog.service
```

### sed

#### Delete lines from file

Remove first line:

```
sed -i "1d" some.txt
```

Remove second line:

```
sed -i "2d" some.txt
```

Remove last line:

```
sed -i "$d" some.txt
```

Remove lines from 2nd to 8th (including 2nd and 8th):

```
sed -i "2,8d" some.txt
```

Remove lines containing `ololo` (regular expression):

```
sed -i "/ololo/d" some.txt
```

#### Replace text in files

```
$ find ./ -type f -exec sed -i 's/ololo/some\/path/g' {} \;
```

* `find ./` look in the current folder
* `-type f` - apply to files
* `sed -i` - replace all the occurrences of `ololo` string with `some/path` string

### Screen

When you need to run some long process, and you're worried that your SSH connection might break, the solution would be to start the `screen` session and then you can detach and reattach to it at any moment. That is especially useful when you do system upgrades.

Start a named session:

```
$ screen -S updating
```

Do your stuff, run the upgrading process, whatever.

You can detach from the session by pressing the combination `Ctrl` + `A` + `D`.

To attach back to it:

```
$ screen -r
```

But if you have several `screen` sessions, then you might need to list them first:

```
$ screen -list

There are screens on:
	27734.updating	(01/22/20 12:16:16)	(Detached)
	27718.pts-0.283746	(01/22/20 12:14:32)	(Detached)
	27706.pts-0.283746	(01/22/20 12:14:00)	(Detached)
3 Sockets in /run/screen/S-root.
```

And then reattach using the session ID:

```
$ screen -r 27734
```

To close some session:

```
$ screen -XS 27706 quit
```

### x509 certificate

To be used in a .NET Core project for [PersistKeysToFileSystem](https://docs.microsoft.com/en-us/aspnet/core/security/data-protection/implementation/key-storage-providers?view=aspnetcore-3.1&tabs=visual-studio#file-system).

```
$ openssl req -x509 -newkey rsa:4096 -sha256 -keyout key.pem -out cert.pem -days 365 -nodes -subj "/CN=YOUR-DOMAIN"
$ openssl pkcs12 -inkey key.pem -in cert.pem -export -out cert.pfx -passout pass:YOUR-PASSWORD
```

### Allow program to bind to 80 port

For example, you want to allow Grafana to bind to 80 port without running it as root:

```
$ setcap 'cap_net_bind_service=+ep' /usr/sbin/grafana-server
```

### Define a variable using configure

If the source code allows you to define some variable on configuration step, here's how you can do that:

```
$ sudo apt install automake autoconf
$ nano configure.ac

AC_DEFINE([SOME_VAR], [9000], [Set some variable to 9000])

$ touch configure.ac
$ ./configure
```

### iptables

All current rules:

```
$ iptables -L
```

Just incoming rules:

```
$ iptables -L INPUT
```

Block incoming requests from some IP:

```
$ iptables -A INPUT -s 178.128.230.58 -j DROP
```

Delete a rule:

```
$ iptables -D INPUT -s 178.128.230.58 -j DROP
```
