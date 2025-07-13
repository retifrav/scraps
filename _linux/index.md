## Linux

<!-- MarkdownTOC -->

- [Versions](#versions)
    - [Linux](#linux)
    - [OpenGL](#opengl)
- [Packages](#packages)
    - [APT](#apt)
        - [Install and reinstall](#install-and-reinstall)
        - [List installed packages](#list-installed-packages)
        - [Update packages](#update-packages)
            - [Upgrading the system](#upgrading-the-system)
        - [Search for a package](#search-for-a-package)
        - [Delete packages](#delete-packages)
            - [Delete Snap](#delete-snap)
        - [Install Firefox without Snap](#install-firefox-without-snap)
    - [dpkg](#dpkg)
        - [Install package](#install-package)
        - [List installed packages](#list-installed-packages-1)
        - [Uninstall package](#uninstall-package)
        - [Edit package contents](#edit-package-contents)
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
    - [Remove user from the group](#remove-user-from-the-group)
    - [List users of the group](#list-users-of-the-group)
    - [Change owner group of the folder](#change-owner-group-of-the-folder)
- [Network](#network)
    - [Interfaces](#interfaces)
    - [Clear DNS cache](#clear-dns-cache)
    - [Set static IP address](#set-static-ip-address)
    - [Renew IP address](#renew-ip-address)
    - [List open ports](#list-open-ports)
    - [Open some ports in firewall](#open-some-ports-in-firewall)
    - [Allow program to bind to 80 port](#allow-program-to-bind-to-80-port)
    - [iptables](#iptables)
        - [Set rules](#set-rules)
        - [Save rules](#save-rules)
    - [Scan local network](#scan-local-network)
- [CPU temperature](#cpu-temperature)
- [Web-servers](#web-servers)
    - [Get web-server version](#get-web-server-version)
    - [NGINX](#nginx)
        - [Logs rotation](#logs-rotation)
    - [Basic authentication](#basic-authentication)
        - [NGINX](#nginx-1)
        - [Apache](#apache)
- [Set up a new server for NET Core deployment](#set-up-a-new-server-for-net-core-deployment)
- [Files and folders](#files-and-folders)
    - [List files](#list-files)
        - [Basic list with most of the information](#basic-list-with-most-of-the-information)
        - [Only the filename](#only-the-filename)
        - [Only the filename and size](#only-the-filename-and-size)
        - [A tree alternative](#a-tree-alternative)
        - [With respect to numbers in filenames](#with-respect-to-numbers-in-filenames)
        - [Flatten the list from nested directories](#flatten-the-list-from-nested-directories)
        - [Find all UTF-16 files](#find-all-utf-16-files)
        - [Flat list of files in nested folders](#flat-list-of-files-in-nested-folders)
    - [Get a list of distinct extensions in a folder](#get-a-list-of-distinct-extensions-in-a-folder)
    - [Get the size of a directory](#get-the-size-of-a-directory)
    - [Create a directory and open it](#create-a-directory-and-open-it)
    - [Do something based on directory existence](#do-something-based-on-directory-existence)
    - [Sync folders](#sync-folders)
    - [Copy one folder contents to another](#copy-one-folder-contents-to-another)
    - [Deleting](#deleting)
        - [Find some files and delete them](#find-some-files-and-delete-them)
        - [Delete empty folders](#delete-empty-folders)
    - [Execute some command for all the files in the folder](#execute-some-command-for-all-the-files-in-the-folder)
    - [Preview ZIP archive contents](#preview-zip-archive-contents)
    - [Count](#count)
    - [Get absolute path from relative path](#get-absolute-path-from-relative-path)
    - [Get the path folder and name](#get-the-path-folder-and-name)
    - [Get the last section of path](#get-the-last-section-of-path)
    - [Fix files permissions](#fix-files-permissions)
    - [Get numerical chmod value](#get-numerical-chmod-value)
    - [Copy files based on a list from text file](#copy-files-based-on-a-list-from-text-file)
    - [Remove duplicate lines from the file](#remove-duplicate-lines-from-the-file)
    - [Watch the progress of a packing operation](#watch-the-progress-of-a-packing-operation)
    - [Get access rights for every section in the path](#get-access-rights-for-every-section-in-the-path)
    - [Symlinks](#symlinks)
    - [Pack several files into individual archives with 7z](#pack-several-files-into-individual-archives-with-7z)
- [Mounting USB drives](#mounting-usb-drives)
    - [Automount media on startup](#automount-media-on-startup)
    - [Safely unmount and eject USB disk](#safely-unmount-and-eject-usb-disk)
- [Build something from source](#build-something-from-source)
- [Get return code](#get-return-code)
- [systemd](#systemd)
    - [Create a new service](#create-a-new-service)
        - [System](#system)
        - [User](#user)
    - [Status of the service](#status-of-the-service)
    - [View log of the service](#view-log-of-the-service)
    - [Limit service log size](#limit-service-log-size)
    - [Restart the service](#restart-the-service)
    - [Reload changed configuration](#reload-changed-configuration)
    - [List all services and their status](#list-all-services-and-their-status)
- [Run commands in background](#run-commands-in-background)
- [GRUB](#grub)
    - [Default boot option](#default-boot-option)
- [Set time zone](#set-time-zone)
- [Cron](#cron)
    - [Enable cron log](#enable-cron-log)
- [screen](#screen)
    - [Start a named session](#start-a-named-session)
    - [Run something in a screen without attaching](#run-something-in-a-screen-without-attaching)
    - [Make sure it is not killed when you disconnect from the server](#make-sure-it-is-not-killed-when-you-disconnect-from-the-server)
- [Define a variable using configure](#define-a-variable-using-configure)
- [List only files from ZIP archive contents](#list-only-files-from-zip-archive-contents)
- [Most frequent commands from Bash history](#most-frequent-commands-from-bash-history)
- [fail2ban](#fail2ban)
    - [Unban some IP address](#unban-some-ip-address)
- [xargs](#xargs)
    - [Lines from a file](#lines-from-a-file)
    - [Results of grep](#results-of-grep)
- [Making a file out of a template by substituting variables](#making-a-file-out-of-a-template-by-substituting-variables)
- [Installing newer JDK](#installing-newer-jdk)
- [Swap and cache](#swap-and-cache)

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

#### APT

##### Install and reinstall

``` sh
$ sudo apt install SOMETHING
```

If you need to reinstall the package and restore its original configs:

``` sh
$ sudo apt install --reinstall \
    -o Dpkg::Options::="--force-confask,confnew,confmiss" \
    SOMETHING
```

##### List installed packages

``` sh
$ apt list --installed
```

##### Update packages

``` sh
$ sudo apt update
$ sudo apt upgrade
$ sudo apt autoremove
```

###### Upgrading the system

``` sh
$ sudo apt install update-manager-core
```

Switch from `lts` to `normal` in `Prompt`:

``` sh
$ sudo nano /etc/update-manager/release-upgrades
```

Start a [screen](#screen) session and run:

``` sh
$ do-release-upgrade
```

##### Search for a package

``` sh
$ sudo apt search ninja-build
Sorting... Done
Full Text Search... Done
ninja-build/focal 1.10.0-1build1 amd64
  small build system closest in spirit to Make
```

List all the available package versions:

``` sh
$ apt list -a ninja-build
Listing... Done
ninja-build/focal 1.10.0-1build1 amd64
```

or:

``` sh
$ apt-cache policy ninja-build
ninja-build:
  Installed: (none)
  Candidate: 1.10.0-1build1
  Version table:
     1.10.0-1build1 500
        500 http://archive.ubuntu.com/ubuntu focal/universe amd64 Packages
```

##### Delete packages

For example, we want to delete **LibreOffice**. All of its packages names start with `libreoffice`, so:

``` sh
$ sudo apt remove --purge libreoffice*
$ sudo apt clean
$ sudo apt autoremove
```

Or, more civilized way, let's remove **minidlna** and its dependencies:

``` sh
$ sudo apt remove --auto-remove minidlna
```

###### Delete Snap

Yeah, [fuck Snap](./snap-ram.png):

``` sh
$ sudo systemctl stop snapd
$ sudo systemctl disable snapd
$ sudo apt autoremove --purge snapd gnome-software-plugin-snap
$ rm -rf ~/snap
$ sudo rm -rf /var/snap /var/cache/snapd /usr/lib/snapd
```

If it gives you something like:

``` sh
rm: cannot remove '/var/snap/firefox/common/host-hunspell/en_US.aff': Read-only file system
rm: cannot remove '/var/snap/firefox/common/host-hunspell/en_US.dic': Read-only file system
```

then do that first:

``` sh
$ sudo umount /var/snap/firefox/common/host-hunspell
```

and then repeat.

##### Install Firefox without Snap

<https://ubuntuhandbook.org/index.php/2022/04/install-firefox-deb-ubuntu-22-04/>

``` sh
$ sudo add-apt-repository ppa:mozillateam/ppa
$ sudo apt update
$ sudo apt install -t 'o=LP-PPA-mozillateam' firefox
```

``` sh
$ sudo nano /etc/apt/preferences.d/mozillateamppa
```
```
Package: firefox*
Pin: release o=LP-PPA-mozillateam
Pin-Priority: 501
```

#### dpkg

##### Install package

``` sh
$ sudo dpkg -i /path/to/somePackage.deb
```

##### List installed packages

``` sh
$ dpkg -l
```

or:

``` sh
$ dpkg --get-selections | grep -v deinstall
```

##### Uninstall package

``` sh
$ sudo dpkg -r some-package
```

##### Edit package contents

Unpack it somewhere:

``` sh
$ dpkg-deb -R ./some-package.deb ./tmp
```

Edit files, add/remove files and pack it back:

``` sh
$ dpkg-deb -b ./tmp ./some-package-new.deb
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

``` sh
$ groups
```
#### List all the groups

``` sh
$ cut -d: -f1 /etc/group | sort
```

#### Create new group

``` sh
$ groupadd NEW-GROUP
```

#### Add user to the group

``` sh
$ usermod -a -G GROUP-NAME USER-NAME
```

#### Remove user from the group

``` sh
$ deluser USER-NAME GROUP-NAME
```

#### List users of the group

``` sh
$ grep NEW-GROUP /etc/group
```

#### Change owner group of the folder

``` sh
$ chgrp -R NEW-GROUP /etc/SOME-FOLDER/
```

### Network

#### Interfaces

Check what network you have:

```
# sudo lshw -C network
```

To turn off Wi-Fi, use its `logical name`:

```
$ sudo ifconfig wlp4s0 down
```

#### Clear DNS cache

```
$ sudo systemd-resolve --flush-caches
```

#### Set static IP address

``` sh
$ sudo nano /etc/netplan/00-installer-config.yam
```
``` yaml
network:
  version: 2
  ethernets:
    ens18:
      dhcp4: true
      addresses: [10.200.16.96/24]
      gateway4: 10.200.16.1
      nameservers:
        addresses: [10.200.16.110]
```
``` sh
$ sudo netplan apply
```

You will lose connection and will need to reconnect.

#### Renew IP address

For example, if host machine has changed the network, and you need to update the IP address in your guest VM:

``` sh
$ dhclient -v -r
```

#### List open ports

``` sh
$ sudo netstat -lntup
```

or

``` sh
$ sudo ss -lntup
```

#### Open some ports in firewall

If your host is inside some cloud provider infrastructure (*for example, Oracle Cloud*), aside from creating routes to external internet from their subnet you might also need to open ports on the host:

```
$ sudo firewall-cmd --zone=public --permanent --add-port=80/tcp
$ sudo firewall-cmd --reload
```

#### Allow program to bind to 80 port

For example, you want to allow Grafana to bind to 80 port without running it as root:

```
$ setcap 'cap_net_bind_service=+ep' /usr/sbin/grafana-server
```

#### iptables

##### Set rules

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

##### Save rules

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

#### Scan local network

``` sh
$ nmap -sP 192.168.1.0/24
```

### CPU temperature

``` sh
$ cat /sys/class/thermal/thermal_zone*/temp
```

or, more useful:

``` sh
$ paste <(cat /sys/class/thermal/thermal_zone*/type) <(cat /sys/class/thermal/thermal_zone*/temp) | column -s $'\t' -t | sed 's/\(.\)..$/.\1°C/'
```

### Web-servers

#### Get web-server version

``` sh
$ curl -s -I example.com|awk '$1~/Server:/ {print $2}'
```

#### NGINX

##### Logs rotation

Log files are split every week and rotated every 8 weeks (2 months).

``` sh
$ sudo nano /etc/logrotate.d/nginx
```
``` nginx
/var/log/nginx/*.log {
    weekly
    missingok
    rotate 2
    maxage 100
    maxsize 100M
    compress
    delaycompress
    notifempty
    create 0640 www-data www-data
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
```

here:

- `weekly` - switch to a new log file each week;
- `rotate 2` - number of files based on rotation value, so here it's 2 *weeks*;
- `maxage 100` - disregarding the rotation period value, how old (*in days*) can a log file be until it gets rotated;
- `maxsize 100M` - disregarding the rotation period value, maximum allowed size in megabytes for a log file until it gets rotated.

After editing the file:

``` sh
$ sudo kill -USR1 $(cat /var/run/nginx.pid)
```

#### Basic authentication

Get password generator:

``` sh
$ sudo apt install apache2-utils
```

Add a new user/password:

``` sh
$ sudo htpasswd -c /etc/nginx/.htpasswd someusername
```

And configure your website to use this file for Basic Authentication.

##### NGINX

``` nginx
location / {
        try_files $uri $uri/ =404;

        auth_basic           "restricted area";
        auth_basic_user_file /etc/nginx/.htpasswd;
}
```

##### Apache

``` apache
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
$ tree .
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

##### Find all UTF-16 files

``` sh
$ find . -type f -exec file --mime {} \; | grep "charset=utf-16"
```

##### Flat list of files in nested folders

The files tree:

``` sh
$ tree .
└── wasm
    ├── cmake
    │     ├── pdfLayerConfig-release.cmake
    │     └── pdfLayerConfig.cmake
    ├── include
    │     ├── Addons
    │     │     └── PdfLayer
    │     │         └── somePDF.h
    │     └── pdfium
    │         ├── fpdf_annot.h
    │         ├── fpdf_attachment.h
    │         ├── fpdf_catalog.h
    │         ├── fpdf_dataavail.h
    │         ├── fpdf_doc.h
    │         ├── fpdf_edit.h
    │         ├── fpdf_ext.h
    │         ├── fpdf_flatten.h
    │         ├── fpdf_formfill.h
    │         ├── fpdf_fwlevent.h
    │         ├── fpdf_javascript.h
    │         ├── fpdf_ppo.h
    │         ├── fpdf_progressive.h
    │         ├── fpdf_save.h
    │         ├── fpdf_searchex.h
    │         ├── fpdf_signature.h
    │         ├── fpdf_structtree.h
    │         ├── fpdf_sysfontinfo.h
    │         ├── fpdf_text.h
    │         ├── fpdf_thumbnail.h
    │         ├── fpdf_transformpage.h
    │         └── fpdfview.h
    ├── lib
    │     ├── libPdfLayer.a
    │     └── libpdfium.a
    └── share
        └── pdfium
            ├── PDFiumConfig-release.cmake
            ├── PDFiumConfig.cmake
            ├── copyright
            ├── vcpkg.spdx.json
            └── vcpkg_abi_info.txt
```

The flatten list:

``` sh
$ find . ! -type d -exec ls -L1 {} +
./wasm/cmake/pdfLayerConfig-release.cmake
./wasm/cmake/pdfLayerConfig.cmake
./wasm/include/Addons/PdfLayer/somePDF.h
./wasm/include/pdfium/fpdf_annot.h
./wasm/include/pdfium/fpdf_attachment.h
./wasm/include/pdfium/fpdf_catalog.h
./wasm/include/pdfium/fpdf_dataavail.h
./wasm/include/pdfium/fpdf_doc.h
./wasm/include/pdfium/fpdf_edit.h
./wasm/include/pdfium/fpdf_ext.h
./wasm/include/pdfium/fpdf_flatten.h
./wasm/include/pdfium/fpdf_formfill.h
./wasm/include/pdfium/fpdf_fwlevent.h
./wasm/include/pdfium/fpdf_javascript.h
./wasm/include/pdfium/fpdf_ppo.h
./wasm/include/pdfium/fpdf_progressive.h
./wasm/include/pdfium/fpdf_save.h
./wasm/include/pdfium/fpdf_searchex.h
./wasm/include/pdfium/fpdf_signature.h
./wasm/include/pdfium/fpdf_structtree.h
./wasm/include/pdfium/fpdf_sysfontinfo.h
./wasm/include/pdfium/fpdf_text.h
./wasm/include/pdfium/fpdf_thumbnail.h
./wasm/include/pdfium/fpdf_transformpage.h
./wasm/include/pdfium/fpdfview.h
./wasm/lib/libPdfLayer.a
./wasm/lib/libpdfium.a
./wasm/share/pdfium/PDFiumConfig-release.cmake
./wasm/share/pdfium/PDFiumConfig.cmake
./wasm/share/pdfium/copyright
./wasm/share/pdfium/vcpkg.spdx.json
./wasm/share/pdfium/vcpkg_abi_info.txt
```

#### Get a list of distinct extensions in a folder

``` sh
$ find . -type f | perl -ne 'print $1 if m/\.([^.\/]+)$/' | sort | uniq -c | sort -n
  1 gitattributes
  1 md
  7 mp4
  8 PNG
 13 sample
 89 jpg
158 png
```

There is also a variant without Perl:

``` sh
$ find . -type f | rev | cut -d. -f1 | rev  | tr '[:upper:]' '[:lower:]' | sort | uniq --count | sort -rn
```

but it won't work on Windows in Git BASH, because there is no `rev` there.

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

#### Copy one folder contents to another

Given this:

``` sh
$ tree .
├── a
│   ├── another
│   │   └── thing.txt
│   └── some.txt
└── b
    ├── another
    │   └── different.txt
    └── ololo.txt
```

to copy contents of `a` to `b`:

``` sh
$ cp -an ./a/* ./b/
```

but if, for whatever reasons, you can't use `cp`, or if it acts differently on other systems/environments, then you could use `rsync` or other similar tool for that, but without those there is just no way. Except for this trick with `tar` (*which should be available in any system*):

``` sh
$ (cd ./a && tar -c .) | (cd ./b && tar -xf -)

$ tree .
├── a
│   ├── another
│   │   └── thing.txt
│   └── some.txt
└── b
    ├── another
    │   ├── different.txt
    │   └── thing.txt
    ├── ololo.txt
    └── some.txt
```

#### Deleting

##### Find some files and delete them

For example, delete all `.php` files from the folder (and all the subfolders).

``` sh
$ find . -type f -name "*.php" -exec rm {} +
```

##### Delete empty folders

``` sh
$ find . -type d -empty -delete
```

#### Execute some command for all the files in the folder

For example, convert line endings with `dos2unix`:

``` sh
$ find . -type f -print0 | xargs -0 dos2unix
```

#### Preview ZIP archive contents

```
$ less archive.zip
```

or

```
$ unzip -l archive.zip | tail -10
```

#### Count

Folders:

On the current level only:

``` sh
$ find . -mindepth 1 -maxdepth 1 -type d | wc -l
```

Files recursively:

``` sh
$ find . -type f -name '*.log' -printf x | wc -c
```

Files non-recursively:

``` sh
$ find . -maxdepth 1 -type f -name '*.log' -printf x | wc -c
```

#### Get absolute path from relative path

``` sh
$ cd ~
$ readlink -f .bash_profile
/home/USERNAME/.bash_profile
```

#### Get the path folder and name

``` sh
$ dirname /var/www/html/index.html
/var/www/html

$ basename /var/www/html/index.html
index.html
```

#### Get the last section of path

If you need just the last section:

``` sh
$ echo "/var/www/html/index.html" | rev | cut -d '/' -f 1 | rev
index.html
```

or, in case of Git BASH on Windows, where there is no `rev`:

``` sh
$ echo "/var/www/html/index.html" | tr '/' '\n' | tail -n1
index.html
```

And if you need the parent folder of this last section:

``` sh
$ echo "/var/www/html/index.html" | rev | cut -d '/' -f 2 | rev
html
```

or:

``` sh
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

#### Copy files based on a list from text file

Flat structure (*will fail if there are files with the same name in different sub-folders*):

``` sh
$ cp $(<list.txt) /path/to/destination/folder
```

Preserving the folder structure and succeeding even if there are missing files:

``` sh
$ cp --parents $(<list.txt) /path/to/destination/folder || :
```

#### Remove duplicate lines from the file

Note that the output will be sorted:

``` sh
$ sort -u ./file-with-duplicates.txt > ./permutted-file.txt
```

Also note that you cannot redirect output to the same file - it will be erased. If you'd like to put results into the same file, then do this:

``` sh
$ sort -u -o ./some-file.txt ./some-file.txt
```

or shorter version:

``` sh
$ sort -u -o ./some-file.txt{,}
```

#### Watch the progress of a packing operation

Packing and compression:

``` sh
$ tar cf - /path/to/folder/to/pack -P | pv -s $(du -sb /path/to/folder/to/pack | awk '{print $1}') | gzip > archive.tar.gz
```

Only packing, no compression:

``` sh
$ tar -c /path/to/folder/to/pack | pv -s $(du -sb /path/to/folder/to/pack | awk '{print $1}') > archive.tar
```

alternative:

``` sh
$ tar cf - /path/to/folder/to/pack -P | pv -s $(du -sb /path/to/folder/to/pack | awk '{print $1}') > archive.tar
```

#### Get access rights for every section in the path

``` sh
$ namei -mov /home/user/some/certificate.pem
```

#### Symlinks

Absolute:

``` sh
$ pwd
/home/vasya/Downloads

$ ln -s /home/vasya/programs/some/executable ../bin/executable
```

Relative:

``` sh
$ pwd
/home/vasya/Downloads

$ ln -sr ../programs/some/executable ../bin/executable
```

On Mac OS for `-r` you'll need to use `gln` (*[GNU ln](https://www.gnu.org/software/coreutils/manual/html_node/ln-invocation.html#ln-invocation)*) instead of `ln`.

#### Pack several files into individual archives with 7z

``` sh
$ cd /some/path/
$ ls -L1 ./*.vul
./ME1MS03_00010H2O-1.vul
./ME1MS03_00010H2O-2.vul
./ME1MS03_00010H2O-3.vul
./ME1MS03_00010H2O-4.vul
./ME1MS03_00010H2O-5.vul
...

$ time for f in ./*.vul; do 7za a "./${f%.*}.7z" "$f"; done
...
real    59m51.181s

$ du -hc ./*.vul
6.6G    ./ME1MS03_00010H2O-1.vul
6.2G    ./ME1MS03_00010H2O-2.vul
6.4G    ./ME1MS03_00010H2O-3.vul
6.3G    ./ME1MS03_00010H2O-4.vul
6.9G    ./ME1MS03_00010H2O-5.vul
...
232G    total

$ du -hc ./*.7z
3.9G    ./ME1MS03_00010H2O-1.7z
3.6G    ./ME1MS03_00010H2O-2.7z
3.7G    ./ME1MS03_00010H2O-3.7z
3.6G    ./ME1MS03_00010H2O-4.7z
3.9G    ./ME1MS03_00010H2O-5.7z
...
129G    total
```

You can try setting different compression level with `-mx3` (*from `0` to `9`, where `9` is the slowest and best compression*), but actually it does just fine with the default level.

### Mounting USB drives

#### Automount media on startup

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

#### Safely unmount and eject USB disk

<https://unix.stackexchange.com/a/83157/254512>

``` sh
$ sudo fdisk -l
$ sudo umount /dev/sdb
$ sudo eject -s /dev/sdb
```

where `sdb` is your disk.

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
returnCode=$?
echo "Exit code: $returnCode"
```

### systemd

#### Create a new service

##### System

Create a config for the new service:

``` sh
$ sudo nano /etc/systemd/system/some.service
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

``` sh
$ sudo systemctl enable some.service
$ sudo systemctl start some.service
```

##### User

Create a config for the new service:

``` sh
$ nano ~/.config/systemd/user/some.service
```
``` ini
[Unit]
Description=cec

[Service]
Environment=DISPLAY=:0
ExecStart=/home/pi/programs/pipe-cec.sh
#Restart=always
#RestartSec=10
SyslogIdentifier=cec
#User=pi

[Install]
WantedBy=default.target
```
``` sh
$ systemctl --user enable some.service
$ systemctl --user start some.service
```

Here:

- `Environment=DISPLAY=:0` - if your service is supposed to interact with the Xorg sessions (*such as emulating user input via `xdotool`*);
- `WantedBy=default.target` - for auto-starting on system boot.

#### Status of the service

System:

``` sh
$ systemctl status some.service
```

User:

``` sh
$ systemctl --user status some.service
```

#### View log of the service

System:

``` sh
$ journalctl -u some.service
```

User:

``` sh
$ journalctl --user -u some.service
```

Navigation:

- `f` - scroll one page down;
- `g` - scroll to the first line;
- `SHIFT` + `g` - scroll the the last line.

Get last 10 lines from the service journal:

``` sh
$ journalctl -u some.service -n 10 --no-pager
```

Watch the new log entries:

``` sh
$ journalctl -u some.service -f
```

#### Limit service log size

<https://andreaskaris.github.io/blog/linux/setting-journalctl-limits/>

``` sh
$ sudo nano /etc/systemd/journald.conf
```
``` ini
[Journal]
# ...
SystemMaxUse=111M
#SystemKeepFree=5G
SystemMaxFileSize=11M
# ...
MaxFileSec=1month
# ...
```
``` sh
$ sudo systemctl restart systemd-journald.service
```

#### Restart the service

System:

``` sh
$ sudo systemctl restart some.service
```

User:

``` sh
$ systemctl --user restart some.service
```

#### Reload changed configuration

``` sh
$ sudo systemctl daemon-reload
```

#### List all services and their status

``` sh
$ service --status-all
```

### Run commands in background

Add `&` to the end of the command in order to run it in background::

``` sh
$ ping ya.ru >> ping.txt &
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

``` sh
$ sudo nano etc/default/grub
```

Set the default option, enumeration starts from 0. To be sure, check the list from the boooting menu.

``` sh
$ sudo update-grub
```

You can also delete unwanted items from `/boot/grub/grub.cfg`.

### Set time zone

``` sh
$ sudo dpkg-reconfigure tzdata
```

### Cron

``` sh
$ crontab -e

# run at 23:01 every 3 days
1 23 */3 * * /root/backup.sh > /dev/null 2>&1
# run at 01:01 on the 1st of every month
1 1 1 * * certbot renew

$ systemctl restart cron.service
```

#### Enable cron log

``` sh
$ sudo nano /etc/rsyslog.d/50-default.conf

# uncomment
cron.*                          /var/log/cron.log

$ sudo systemctl restart rsyslog.service
```

### screen

When you need to run some long process, and you're worried that your SSH connection might break, the solution would be to start the `screen` session and then you can detach and reattach to it at any moment. That is especially useful when you do system upgrades.

#### Start a named session

```
$ screen -S updating
```

Do your stuff, run the upgrading process, whatever.

You can detach from the session by pressing the combination `Ctrl` + `A` + `D`.

To attach back to it:

``` sh
$ screen -r
```

But if you have several `screen` sessions, then you might need to list them first:

``` sh
$ screen -list

There are screens on:
	27734.updating	(01/22/20 12:16:16)	(Detached)
	27718.pts-0.283746	(01/22/20 12:14:32)	(Detached)
	27706.pts-0.283746	(01/22/20 12:14:00)	(Detached)
3 Sockets in /run/screen/S-root.
```

And then reattach using the session ID:

``` sh
$ screen -r 27734
```

To close a session:

``` sh
$ screen -XS 27706 quit
```

#### Run something in a screen without attaching

``` sh
$ screen -dmS ydl yt-dlp https://youtu.be/dQw4w9WgXcQ
```

here:

- `-d -m` - start screen in detached mode
- `-S ydl` - name the session `ydl`, not required

#### Make sure it is not killed when you disconnect from the server

At some point some motherfucker changed something in the default behavior of systemd, or perhaps motherfucking systemd is the reason, but anyway, you'll be surprised to discover that your screen sessions are fucking killed as soon as you are disconnected from the server, which defeats the whole motherfucking point.

I don't know what exactly from the following returned the normal behavior, but something did:

``` sh
$ loginctl enable-linger USERNAME
$ ls /var/lib/systemd/linger
```

or/and:

``` sh
$ sudo nano /etc/systemd/logind.conf
```
``` ini
[Login]
#NAutoVTs=6
#ReserveVT=6
KillUserProcesses=no
#KillOnlyUsers=
KillExcludeUsers=root USERNAME
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

### List only files from ZIP archive contents

```
$ lesspipe /path/to/some.zip | tail --lines=+4 | head --lines=-2 | awk '{print $NF}'
```

Though, it seems to use spaces as separators, so listing might be incorrect in case of space in paths.

### Most frequent commands from Bash history

```
$ cat ~/.bash_history | sort | uniq -c | sort -n
```

### fail2ban

Block those bastards from brute-forcing your server. A detailed guide: <https://www.linode.com/docs/guides/how-to-use-fail2ban-for-ssh-brute-force-protection/>.

``` sh
$ sudo apt install fail2ban

$ sudo nano /etc/fail2ban/jail.local
[sshd]
enabled = true
port = ssh
filter = sshd
logpath = /var/log/auth.log
maxretry = 3
findtime = 667
bantime = 11111
ignoreip = 127.0.0.1

$ sudo systemctl enable fail2ban.service
$ sudo systemctl start fail2ban.service

$ sudo fail2ban-client status
$ sudo fail2ban-client status sshd
```

#### Unban some IP address

``` sh
$ sudo fail2ban-client set sshd unbanip 1.2.3.4
```

### xargs

Use output from the previous command as an argument(s) for the next one in pipe.

#### Lines from a file

Read file and use all of its lines as one argument:

``` sh
$ cat ./some.txt
ololo
fuuu
some
another
thing
and
TEH END

$ cat ./some.txt | xargs echo "rrrargh"
rrrargh ololo fuuu some another thing and TEH END
```

To use every line in the file as an argument, so to execute the next command for every line, use placeholder/replacement:

``` sh
$ cat ./some.txt | xargs -I {} echo "{} rrrargh"
ololo rrrargh
fuuu rrrargh
some rrrargh
another rrrargh
thing rrrargh
and rrrargh
TEH END rrrargh
```

#### Results of grep

Find all the files (*except for `.pyc`*) that contain `someuser` string and replace certain paths in those files with `sed`:

``` sh
$ grep -irnl --exclude \*.pyc -e "someuser" \
    | xargs -I {} sed -i 's/\/Users\/someuser\/code\/python\/_venvs\/altaipony\//\/home\/anotheruser\/_venvs\/altaipony\//g' {}
```

### Making a file out of a template by substituting variables

A template file `some.template`:

```
Here comes first variable value: "$SOME_VARIABLE".

And lastly, another thing: $ANOTHER_VARIABLE
```

And those variables can be now substituted with `envsubst`:

``` sh
$ export SOME_VARIABLE="some value"
$ export ANOTHER_VARIABLE="something else"

$ envsubst < /path/to/some.template > /path/to/some.txt
```

or:

``` sh
$ SOME_VARIABLE="some value" ANOTHER_VARIABLE="something else" envsubst < /path/to/some.template > /path/to/resulting.txt
```

### Installing newer JDK

Such as if you have Java 8, but you need Java 11:

``` sh
$ java --version
$ echo $JAVA_HOME

$ sudo apt update
$ sudo apt install openjdk-11-jdk

$ sudo update-java-alternatives --list
java-1.11.0-openjdk-amd64      1111       /usr/lib/jvm/java-1.11.0-openjdk-amd64
java-1.8.0-openjdk-amd64       1081       /usr/lib/jvm/java-1.8.0-openjdk-amd64

$ sudo update-alternatives --config java
There are 2 choices for the alternative java (providing /usr/bin/java).

  Selection    Path                                            Priority   Status
------------------------------------------------------------
* 0            /usr/lib/jvm/java-11-openjdk-amd64/bin/java      1111      auto mode
  1            /usr/lib/jvm/java-11-openjdk-amd64/bin/java      1111      manual mode
  2            /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java   1081      manual mode

Press <enter> to keep the current choice[*], or type selection number:

$ nano ~/.bash_profile
export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64

$ source ~/.bash_profile
$ java --version
$ echo $JAVA_HOME
```

### Swap and cache

<https://www.digitalocean.com/community/tutorials/how-to-add-swap-space-on-ubuntu-20-04>

Check if you already have it enabled:

``` sh
$ sudo swapon --show
```

It it outputs nothing, then you don't. This is also the case if you have `0` for `Swap` row here:

``` sh
$ free -h
               total        used        free      shared  buff/cache   available
Mem:           857Mi       406Mi        66Mi        89Mi       384Mi       215Mi
Swap:             0B          0B          0B
```

If you have 1 GB of RAM on your server, then swap of 1 GB is reasonable (*you can make it twice as big, if you want, or if you are building Qt / something massive, then you might need to set it to 4 GB or even more*):

``` sh
# there is also an option of using `sudo fallocate -l 1G /swapfile`, but it isn't recommended: https://askubuntu.com/a/1177620
$ sudo dd if=/dev/zero of=/swapfile bs=1M count=1024 oflag=append conv=notrunc

$ ls -lh /swapfile
-rw-r--r-- 1 root root 1.0G Jan  9 11:15 /swapfile
$ sudo chmod 600 /swapfile
$ ls -lh /swapfile
-rw------- 1 root root 1.0G Jan  9 11:15 /swapfile

$ sudo mkswap /swapfile
Setting up swapspace version 1, size = 1024 MiB (1073737728 bytes)
no label, UUID=43a09269-dc36-4e51-901d-a3450e7413e6
$ sudo swapon /swapfile
$ sudo swapon --show
NAME      TYPE  SIZE USED PRIO
/swapfile file 1024M   0B   -2

$ free -h
               total        used        free      shared  buff/cache   available
Mem:           857Mi       387Mi        83Mi        89Mi       386Mi       235Mi
Swap:          1.0Gi          0B       1.0Gi
```

It will exist for the current section, but will not persist between reboots, so you need make it permanent:

``` sh
$ cat /etc/fstab
LABEL=cloudimg-rootfs    /     ext4    discard,errors=remount-ro    0 1
LABEL=UEFI    /boot/efi    vfat    umask=0077    0 1

$ echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab

$ cat /etc/fstab
LABEL=cloudimg-rootfs    /     ext4    discard,errors=remount-ro    0 1
LABEL=UEFI    /boot/efi    vfat    umask=0077    0 1
/swapfile none swap sw 0 0
```

Some more settings for swap and cache:

``` sh
$ cat /proc/sys/vm/swappiness
60

$ cat /proc/sys/vm/vfs_cache_pressure
100

$ sudo sysctl vm.swappiness=10
$ sudo sysctl vm.vfs_cache_pressure=50
```

These also need to be made permanent to persist between reboots:

``` sh
$ sudo nano /etc/sysctl.conf
```
``` ini
vm.swappiness=10
vm.vfs_cache_pressure=50
```
