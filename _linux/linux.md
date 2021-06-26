## Linux

<!-- MarkdownTOC -->

- [Versions](#versions)
    - [Linux](#linux)
    - [OpenGL](#opengl)
- [Packages](#packages)
    - [Update packages](#update-packages)
    - [Delete packages](#delete-packages)
        - [Delete Snap](#delete-snap)
- [Upgrading the system](#upgrading-the-system)
- [Users](#users)
    - [All users in the system](#all-users-in-the-system)
    - [Create a new user](#create-a-new-user)
    - [Change your password](#change-your-password)
    - [Last logon](#last-logon)
    - [Allow user to run certain sudo commands](#allow-user-to-run-certain-sudo-commands)
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
        - [Basic list with most of the information](#basic-list-with-most-of-the-information)
        - [Only the filename](#only-the-filename)
        - [Only the filename and size](#only-the-filename-and-size)
        - [A tree alternative](#a-tree-alternative)
        - [With respect to numbers in filenames](#with-respect-to-numbers-in-filenames)
        - [Flatten the list from nested directories](#flatten-the-list-from-nested-directories)
    - [Get the size of a directory](#get-the-size-of-a-directory)
    - [Create a directory and open it](#create-a-directory-and-open-it)
    - [Do something based on directory existence](#do-something-based-on-directory-existence)
    - [Sync folders](#sync-folders)
    - [Find some files and delete them](#find-some-files-and-delete-them)
    - [Preview ZIP archive contents](#preview-zip-archive-contents)
    - [Count folders](#count-folders)
    - [Get the path folder and name](#get-the-path-folder-and-name)
    - [Get the last section of path](#get-the-last-section-of-path)
    - [Fix files permissions](#fix-files-permissions)
    - [Get numerical chmod value](#get-numerical-chmod-value)
    - [Encrypt a folder with PGP](#encrypt-a-folder-with-pgp)
- [Working with FTP](#working-with-ftp)
    - [ftp](#ftp)
    - [lftp](#lftp)
- [Network](#network)
    - [Open ports](#open-ports)
    - [Scan local network](#scan-local-network)
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
    - [Set rules](#set-rules)
    - [Save rules](#save-rules)
- [Diff and patch files](#diff-and-patch-files)
- [Network interfaces](#network-interfaces)
- [Clear DNS cache](#clear-dns-cache)
- [Pipe URL from Python script to cURL](#pipe-url-from-python-script-to-curl)
- [List only files from ZIP archive contents](#list-only-files-from-zip-archive-contents)
- [Base64](#base64)
    - [Encode](#encode)
    - [Decode](#decode)
    - [Image to Base64](#image-to-base64)
- [Open ports in firewall](#open-ports-in-firewall)
- [Most frequent commands from Bash history](#most-frequent-commands-from-bash-history)
- [awk](#awk)
    - [Filter a list](#filter-a-list)

<!-- /MarkdownTOC -->

### Versions

#### Linux

Best way:

```
$ lsb_release -a
```

Good way:

```
$ cat /etc/*-release
```

Kernel and stuff:

```
$ uname -a
```

More stuff:

```
$ cat /proc/version
```

#### OpenGL

```
$ glxinfo | grep "OpenGL version"
```

### Packages

#### Update packages

```
$ sudo apt update
$ sudo apt upgrade
$ sudo apt dist-upgrade
$ sudo apt autoremove
```

#### Delete packages

For example, we want to delete **LibreOffice**. All of its packages names start with `libreoffice`, so:

```
$ sudo apt remove --purge libreoffice*
$ sudo apt clean
$ sudo apt autoremove
```

Or, more civilized way, let's remove **minidlna** and its dependencies:

```
$ sudo apt remove --auto-remove minidlna
```

##### Delete Snap

Yeah, fuck Snap:

```
$ sudo rm -rf /var/cache/snapd/
$ sudo apt autoremove --purge snapd gnome-software-plugin-snap
$ rm -rf ~/snap
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
$ do-release-upgrade
```

### Users

#### All users in the system

```
$ cat /etc/passwd | awk -F ':' '{ print $1 }'
```

#### Create a new user

With `home` directory and change his password.

```
$ useradd -m vasya
$ passwd vasya
```

Another option, if you want a system user for some service needs:

```
$ adduser \
   --system \
   --group \
   --disabled-password \
   --home /home/vasya \
   vasya
```

If later you'll want to be able to "login" as this user:

```
$ sudo usermod -s /bin/bash vasya
$ sudo --login --user vasya
```

#### Change your password

```
$ passwd
```

#### Last logon

```
$ lastlog
```

#### Allow user to run certain sudo commands

Say, you have a user `teamcity` and you want to allow it to restart certain [systemd](#systemd) service. Edit the following file as root:

```
$ sudo nano /etc/sudoers.d/teamcity

%teamcity ALL= NOPASSWD: /bin/systemctl restart some.service
```

Now log-in as that user and test new rights:

```
root@somehost:~# sudo --login --user teamcity
teamcity@somehost:~$ sudo systemctl restart some.service
```

### Groups

#### List current user groups

```
$ groups
```
#### List all the groups

```
$ cut -d: -f1 /etc/group | sort
```

#### Create new group

```
$ groupadd NEW-GROUP
```

#### Add user to the group

```
$ usermod -a -G NEW-GROUP USERNAME
```

#### List users of the group

```
$ grep NEW-GROUP /etc/group
```

#### Change owner group of the folder

```
$ chgrp -R NEW-GROUP /etc/SOME-FOLDER/
```

### Renew IP address

For example, if host machine has changed the network, and you need to update the IP address in your guest VM:

```
$ dhclient -v -r
```

### CPU temperature

```
$ cat /sys/class/thermal/thermal_zone*/temp
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
        maxage 90
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

- `weekly` - switch to a new log file each week
- `rotate 8` - number of files based on rotation value, so here it's 8 *weeks*
- `maxage 90` - disregarding rotation value, number of days to keep files, so here it's 90 *days*

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
$ for f in ./*-main.png; do convert -verbose -quality 50 "$f" "${f%.*}-thumb.jpg"; done
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

##### Basic list with most of the information

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

##### Only the filename

```
$ ls -A1
```

Reverse order:

```
$ ls -A1r
```

##### Only the filename and size

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

##### A tree alternative

```
$ find . | sed -e "s/[^-][^\/]*\//  |/g" -e "s/|\([^ ]\)/|-\1/"
```

##### With respect to numbers in filenames

You have files named `image01.png`, `image02.png`, `image09.png`, `image10.png`, `image11.png` and so on. You need to list them sorted by the name, but also respecting the numbers order, and do some action on each, for example update modification timestamp attribute.

You can do it with pipe and `sort`:

```
$ for f in `ls ./*.png | sort -V`; do touch "$f" && sleep 1; done
```

or right with `ls`:

```
$ for f in `ls -v ./*.png`; do touch "$f" && sleep 1; done
```

##### Flatten the list from nested directories

If you have something like:

``` sh
$ tree . | head -22
.
├── 01\ Jan\ 1999
│   └── 01_01_01.mp3
├── 02\ Feb\ 1999
│   ├── 01_02_01.mp3
│   ├── 01_02_02.mp3
│   ├── 01_02_03.mp3
│   ├── 01_02_04.mp3
│   └── 01_02_05.mp3
├── 03\ Aug\ 1999
│   └── 01_03_01.mp3
├── 04\ Aug\ 1999
│   ├── 01_04_01.mp3
│   ├── 01_04_02.mp3
│   ├── 01_04_03.mp3
│   ├── 01_04_04.mp3
│   ├── 01_04_05.mp3
│   └── 01_04_06.mp3
├── 05\ Mar\ 2000
│   └── 01_05_01.mp3
├── 06\ Apr\ 2000
│   ├── 01_06_01.mp3
```

Then you can flatten this structure this way:

``` sh
$ find . ! -type d -exec gls -1v {} +
'./01 Jan 1999/01_01_01.mp3'
'./02 Feb 1999/01_02_01.mp3'
'./02 Feb 1999/01_02_02.mp3'
'./02 Feb 1999/01_02_03.mp3'
'./02 Feb 1999/01_02_04.mp3'
'./02 Feb 1999/01_02_05.mp3'
'./03 Aug 1999/01_03_01.mp3'
'./04 Aug 1999/01_04_01.mp3'
'./04 Aug 1999/01_04_02.mp3'
'./04 Aug 1999/01_04_03.mp3'
'./04 Aug 1999/01_04_04.mp3'
'./04 Aug 1999/01_04_05.mp3'
'./04 Aug 1999/01_04_06.mp3'
'./05 Mar 2000/01_05_01.mp3'
'./06 Apr 2000/01_06_01.mp3'
```

#### Get the size of a directory

```
$ du -hs /path/to/directory
```

* `-h` - human-readable size
* `-s` - summary, shows the total size only for that directory, otherwise it will show it for all the child ones too

#### Create a directory and open it

```
$ mkdir ololo && cd "$_"
```
* `$_` - special parameter that holds the last *argument* of the previous command

#### Do something based on directory existence

```
$ [ -d "somedir" ] && echo "directory exists" || echo "directory does not exist"
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
$ find . -mindepth 1 -maxdepth 1 -type d | wc -l
```

#### Get the path folder and name

```
$ dirname /var/www/html/index.html
/var/www/html

$ basename /var/www/html/index.html
index.html
```

#### Get the last section of path

If you need just the last section:

```
$ echo "/var/www/html/index.html" | rev | cut -d '/' -f 1 | rev
index.html
```

or, in case of Git BASH on Windows, where there is no `rev`:

```
$ echo "/var/www/html/index.html" | tr '/' '\n' | tail -n1
index.html
```

And if you need the parent folder of this last section:

```
$ echo "/var/www/html/index.html" | rev | cut -d '/' -f 2 | rev
html
```

or:

```
$ dirname "/var/www/html/index.html" | tr '/' '\n' | tail -n1
html
```

#### Fix files permissions

```
find /home/user -type d -print0 | xargs -0 chmod 0775
find /home/user -type f -print0 | xargs -0 chmod 0664
```

#### Get numerical chmod value

```
$ stat -c %a ~/.ssh/github
600
```

#### Encrypt a folder with PGP

Pack the folder into a single archive file (*no compression*):

``` sh
$ tar -C /path/to/folder/to/encrypt -cf archName.tar .
```

If you haven't already, export your public key on local machine (*where your keychain is*), transfer it to the server and import it there:

``` sh
$ gpg --import your-public-key.asc 
```

Now you can encrypt the archive:

``` sh
$ gpg --encrypt --recipient your@email.com archName.tar
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

#### Set rules

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

#### Save rules

Install this thing:

```
$ sudo apt install iptables-persistent
```

and then either:

```
$ sudo /etc/init.d/iptables-persistent save
$ sudo /etc/init.d/iptables-persistent reload
```

or:

```
$ sudo netfilter-persistent save
$ sudo netfilter-persistent reload
```

If saved rules (`/etc/iptables/rules.v4`) are not restored after reboot, then perhaps the service loading order is wrong. Add `iptables.service` and `ip6tables.service` both to `Wants` and `Before` in `/usr/lib/systemd/system/netfilter-persistent.service`:

``` ini
...
Wants=network-pre.target systemd-modules-load.service local-fs.target iptables.service ip6tables.service
Before=network-pre.target shutdown.target iptables.service ip6tables.service
...
```

### Diff and patch files

Suppose you got `1.txt` from someone. You created a copy of it (`2.txt`) and made some changes there. You can now compare these two files and create a patch based on their differences:

```
$ diff 1.txt 2.txt > 3.patch
```

Send your patch to the person with the original `1.txt`. He will then be able to apply your patch and get your `2.txt`:

```
$ patch 1.txt 3.patch
```

If he gets errors like:

```
Hunk #1 FAILED at 1 (different line endings).
Hunk #2 FAILED at 3 (different line endings).
Hunk #3 FAILED at 6 (different line endings).
Hunk #4 FAILED at 8 (different line endings).
```

then after running this:

```
$ dos2unix 3.patch
```

the patch should be able to apply.

### Network interfaces

Check what network you have:

```
# sudo lshw -C network
```

To turn off Wi-Fi, use its `logical name`:

```
$ sudo ifconfig wlp4s0 down
```

### Clear DNS cache

```
$ sudo systemd-resolve --flush-caches
```

### Pipe URL from Python script to cURL

There is some API, we need to get an URL from its JSON response and download a file from that URL.

```
$ jsonContent=$(curl -s -H "Authorization: Bearer ACCESS-TOKEN" http://some.host/api/v1/some/content)
$ echo $jsonContent | python -c "import sys, json; sys.stdout.write('{0}{1}'.format('http://some.host/files/', json.load(sys.stdin)['content']['SDK']['Windows']['links']['MSVC 2019']))" | xargs curl
-s -H "Authorization: Bearer ACCESS-TOKEN" --write-out "%{http_code}" -O
```

### List only files from ZIP archive contents

```
$ lesspipe /path/to/some.zip | tail --lines=+4 | head --lines=-2 | awk '{print $NF}'
```

Though, it seems to use spaces as separators, so listing might be incorrect in case of space in paths.

### Base64

#### Encode

Original string: `s0me-pa$$w0rd-01010`.

Several ways to encode it to Base64:

```
$ echo -n "s0me-pa$$w0rd-01010" | base64
czBtZS1wYTE3MTF3MHJkLTAxMDEw

$ echo "s0me-pa$$w0rd-01010" | tr -d "\n" | base64
czBtZS1wYTE3MTF3MHJkLTAxMDEw

$ python -c "import base64; print(base64.b64encode(b's0me-pa$$w0rd-01010').decode('ascii'))"
czBtZS1wYTE3MTF3MHJkLTAxMDEw

$ echo -n s0me-pa$$w0rd-01010 | openssl enc -base64 -A
czBtZS1wYTE3MTF3MHJkLTAxMDEw
```

Note that Node, however, produces a different result:

```
$ node -e "require('readline') .createInterface({input:process.stdin,output:process.stdout,historySize:0}) .question('PAT> ',p => { b64=Buffer.from(p.trim()).toString('base64');console.log(b64);process.exit(); })"
PAT> s0me-pa$$w0rd-01010
czBtZS1wYSQkdzByZC0wMTAxMA==
```

And actually this is the only correct result, because original string contains special characters and those need to be escaped before encoding, for example:

```
echo -n "s0me-pa\$\$w0rd-01010" | base64
czBtZS1wYSQkdzByZC0wMTAxMA==
```

And since there doesn't seem to be a (*convenient enough*) way of handling such escaping automatically, better to put such strings into a file (`tmp.txt`) and encode the file:

```
$ echo -n $(cat tmp.txt) | base64
czBtZS1wYSQkdzByZC0wMTAxMA==
```

#### Decode

Encoded string: `czBtZS1wYSQkdzByZC0wMTAxMA==`.

Several ways to decode it:

```
$ base64 -d <<< "czBtZS1wYSQkdzByZC0wMTAxMA=="
s0me-pa$$w0rd-01010

$ python -c "import base64; print(base64.b64decode(b'czBtZS1wYSQkdzByZC0wMTAxMA==').decode('ascii'))"
s0me-pa$$w0rd-01010
```

#### Image to Base64

Image (*or any other file really*) can be encoded into a Base64 string:

```
$ base64 -w0 some.png > encoded.txt
```

### Open ports in firewall

If your host is inside some cloud provider infrastructure (*for example, Oracle Cloud*), aside from creating routes to external internet from their subnet you might also need to open ports on the host:

```
$ sudo firewall-cmd --zone=public --permanent --add-port=80/tcp
$ sudo firewall-cmd --reload
```

### Most frequent commands from Bash history

```
$ cat ~/.bash_history | sort | uniq -c | sort -n
```

### awk

#### Filter a list

To show only open ports for IPv4 protocol:

```
$ netstat -lntup | awk '{ if (($1 == "tcp") && ($6 == "LISTEN")) { print $1, " | ", $4, " | ", $6 } }'
tcp  |  0.0.0.0:22  |  LISTEN
tcp  |  127.0.0.1:631  |  LISTEN
tcp  |  127.0.0.1:25  |  LISTEN
tcp  |  127.0.0.1:8890  |  LISTEN
tcp  |  127.0.0.1:49020  |  LISTEN
```
