## Monitoring network traffic

<https://techrepublic.com/article/how-to-monitor-snmp-traffic-on-ubuntu-for-free-with-mrtg/>

<!-- MarkdownTOC -->

- [SNMP](#snmp)
- [MRTG](#mrtg)
    - [systemd](#systemd)

<!-- /MarkdownTOC -->

### SNMP

``` sh
$ sudo apt install snmpd snmp
$ sudo nano /etc/snmp/snmpd.conf
```
```
# ...

sysLocation    Somewhere
sysContact     admin <admin@YOUR.HOST>

# ...

agentaddress  127.0.0.1,[::1]

# ...

#rocommunity  public default -V systemonly
rocommunity  public localhost
#rocommunity6 public default -V systemonly
rocommunity6 public localhost

# ...
```

It is important to replace the `default` value and remove the `-V systemonly` argument, otherwise [mrtg](#mrtg) will generate a config without any interfaces.

Restart the service:

``` sh
$ sudo systemctl restart snmpd.service
```

### MRTG

``` sh
$ sudo apt install mrtg
$ sudo mkdir /etc/mrtg
$ sudo cfgmaker public@localhost > /etc/mrtg/mrtg.cfg

$ sudo chown -R mrtg:ubuntu /etc/mrtg
$ ls -lah /etc/mrtg
total 12K
drwxrws---   2 mrtg ubuntu 4.0K Oct 12 14:10 .
drwxr-xr-x 117 root root   4.0K Oct 10 20:25 ..
-rw-rw-r--   1 mrtg ubuntu 3.9K Oct 11 21:40 mrtg.cfg
```

If your non-root user doesn't have access rights, then:

``` sh
$ sudo su
$ cfgmaker public@localhost > /etc/mrtg/mrtg.cfg
$ exit
```

Create a working directory for HTML reports and charts:

``` sh
$ sudo mkdir /var/www/YOUR.HOST/admin/mrtg
$ sudo nano /etc/mrtg/mrtg.cfg
```
```
WorkDir: /var/www/YOUR.HOST/admin/mrtg
```

Generate an index page:

``` sh
$ sudo indexmaker /etc/mrtg/mrtg.cfg > /var/www/YOUR.HOST/admin/mrtg/index.html
```

If you get:

```
ERROR: did not find any matching data in cfg file
```

Then check what's inside `/etc/mrtg/mrtg.cfg`. If it's all commented (*check the reason for why it is commented - should be stated right there*), then either uncomment the interface of interest (*that is unlikely to be localhost, scroll down to the actual interface*) or try to regenerate the config with `-zero-speed=100000000` to set 12.5 MB/s (*or whichever rate your network connection has on that interface*):

``` sh
$ sudo cfgmaker -zero-speed=100000000 public@localhost > /etc/mrtg/mrtg.cfg
--base: Get Device Info on public@localhost:
--base: Vendor Id: Unknown Vendor - 1.3.6.1.4.1.8072.3.2.10
--base: Populating confcache
--base: Get Interface Info
--base: Walking ifIndex
--snpd:   public@localhost: -> 1 -> ifIndex = 1
--snpd:   public@localhost: -> 2 -> ifIndex = 2
--base: Walking ifType
--snpd:   public@localhost: -> 1 -> ifType = 24
--snpd:   public@localhost: -> 2 -> ifType = 6
--base: Walking ifAdminStatus
--snpd:   public@localhost: -> 1 -> ifAdminStatus = 1
--snpd:   public@localhost: -> 2 -> ifAdminStatus = 1
--base: Walking ifOperStatus
--snpd:   public@localhost: -> 1 -> ifOperStatus = 1
--snpd:   public@localhost: -> 2 -> ifOperStatus = 1
--base: Walking ifMtu
--snpd:   public@localhost: -> 1 -> ifMtu = 65536
--snpd:   public@localhost: -> 2 -> ifMtu = 9000
--base: Walking ifSpeed
--snpd:   public@localhost: -> 1 -> ifSpeed = 10000000
--snpd:   public@localhost: -> 2 -> ifSpeed = 0
```

If nothing is commented out, so there is simply no a single network interface listed, then there is probably something wrong with the `/etc/snmp/snmpd.conf`.

Don't forget to correct the `WorkDir` in `/etc/mrtg/mrtg.cfg`. And maybe also set `EnableIPv6: yes`, while you are there.

Now the index page should generate just fine:

``` sh
$ sudo indexmaker /etc/mrtg/mrtg.cfg > /var/www/YOUR.HOST/admin/mrtg/index.html
```

At first there will be just `index.html` there, but a minute later or less there will be also other files. If not, try to run it manually and see what errors are there:

``` sh
$ env LANG=C /usr/bin/mrtg /etc/mrtg/mrtg.cfg
2025-10-12 13:53:44: ERROR: Creating templock /etc/mrtg/mrtg.cfg_l_224875: Permission denied at /usr/bin/mrtg line 2091.
```

Don't know what the fuck does it want, it has been working fine until I needed to install it on a new server, and there it did not, so I had to create the folder manually:

``` sh
$ sudo mkdir /var/lock/mrtg
$ sudo chown -R mrtg:mrtg /var/lock/mrtg
```

When the files are finally generated, make them available to your web-server:

``` sh
$ sudo chown -R mrtg:www-data /var/www/YOUR.HOST/admin/mrtg
$ ls -lah /var/www/YOUR.HOST/admin/mrtg
total 144K
drwxr-xr-x 2 mrtg     www-data 4.0K Oct 12 14:15 .
drwxr-xr-x 4 teamcity www-data 4.0K Oct 10 15:30 ..
-rw-r--r-- 1 mrtg     www-data 2.4K Oct 11 21:40 index.html
-rw-r--r-- 1 mrtg     mrtg     1.8K Oct 12 14:15 localhost_eth0-day.png
-rw-r--r-- 1 mrtg     www-data 1.5K Oct 12 13:52 localhost_eth0-month.png
-rw-r--r-- 1 mrtg     www-data 1.5K Oct 12 13:52 localhost_eth0-week.png
-rw-r--r-- 1 mrtg     www-data 1.8K Oct 12 13:52 localhost_eth0-year.png
-rw-r--r-- 1 mrtg     www-data 6.1K Oct 12 14:15 localhost_eth0.html
-rw-r--r-- 1 mrtg     mrtg      48K Oct 12 14:15 localhost_eth0.log
-rw-r--r-- 1 mrtg     mrtg      48K Oct 12 14:12 localhost_eth0.old
-rw-r--r-- 1 mrtg     www-data  538 Oct 12 13:52 mrtg-l.png
-rw-r--r-- 1 mrtg     www-data  414 Oct 12 13:52 mrtg-m.png
-rw-r--r-- 1 mrtg     www-data 1.8K Oct 12 13:52 mrtg-r.png
```

or/and add `mrtg` user to `www-data` group:

``` sh
$ sudo usermod -a -G www-data mrtg
```

And probably protect that route [with a password](/_linux/index.md#basic-authentication).

The reports should be available at <https://YOUR.HOST/admin/mrtg/>.

#### systemd

I don't know who runs it by default, apparently someone does, but at one of my new servers apparently no one did (*or maybe it was failing with some error*), so I created an explicit [systemd service](https://github.com/inex/IXP-Manager/issues/627):

``` sh
$ sudo nano /etc/systemd/system/mrtg.service
```
``` ini
[Unit]
Description=MRTG
After=syslog.target

[Service]
User=mrtg
Environment=LANG=C
ExecStart=/usr/bin/mrtg --daemon /etc/mrtg/mrtg.cfg --logging /var/log/mrtg/mrtg.log --pid-file=/run/mrtg/mrtg.pid --lock-file /var/lock/mrtg/mrtg_l --confcache-file /var/lib/mrtg/mrtg.ok
Type=forking
SuccessExitStatus=0 1
RuntimeDirectory=mrtg
PIDFile=/run/mrtg/mrtg.pid
SyslogIdentifier=mrtg

[Install]
WantedBy=multi-user.target
```

If it fails with:

``` sh
cannot write to /run/mrtg/mrtg.pid
```

then you need to also create this:

``` sh
$ sudo mkdir /run/mrtg
$ sudo chown -R mrtg:mrtg /run/mrtg
```

But actually that is only a temporary fix, because after the next reboot the `/run/mrtg/` and `/var/lock/mrtg/` will be missing, and the service will start to fail again. This is apparently because of the [Debian policy](https://debian.org/doc/debian-policy/ch-opersys.html#fr65), which says that `/var/run` and `/var/lock` may be mounted as temporary filesystems, and so one would need to add some crutches for creating them as `ExecStartPre` step (*don't actually do it, keep reading*):

``` sh
$ sudo mkdir /home/mrtg
$ sudo chown -R mrtg:mrtg /home/mrtg
$ sudo nano /home/mrtg/create-required-folders.sh
```
``` sh
#!/bin/bash

sudo mkdir -p /run/mrtg
sudo chown -R mrtg:mrtg /run/mrtg

sudo mkdir -p /var/lock/mrtg
sudo chown -R mrtg:mrtg /var/lock/mrtg
```
``` sh
$ sudo chmod +x /home/mrtg/create-required-folders.sh
```

and then use that script with `ExecStartPre`:

``` sh
$ sudo nano /etc/systemd/system/mrtg.service
```
``` ini
# ...
[Service]
User=mrtg
Environment=LANG=C
ExecStartPre=/home/mrtg/create-required-folders.sh
ExecStart=/usr/bin/mrtg # ...
# ...
```
``` sh
$ sudo systemctl daemon-reload
$ sudo systemctl restart mrtg.service
```

...buuuut that will fail, because `mrtg` user won't be able to execute `sudo mkdir` and `sudo chown`, so creating those folders should be done [differently](https://unix.stackexchange.com/questions/2109/create-directory-in-var-run-at-startup) (*that you can do now*):

``` sh
$ sudo nano /etc/rc.local
```
``` sh
#!/bin/sh -e
# 
# rc.local
#
# This script is executed at the end of each multiuser runlevel.
# Make sure that the script will "exit 0" on success or any other
# value on error.
#
# In order to enable or disable this script just change the execution
# bits.
#
# By default this script does nothing.

if [ ! -d /run/mrtg ]; then
    mkdir /run/mrtg    
    chown mrtg:mrtg /run/mrtg
fi

if [ ! -d /var/lock/mrtg ]; then
    mkdir /var/lock/mrtg 
    chown mrtg:mrtg /var/lock/mrtg
fi

exit 0
```
``` sh
$ sudo chmod +x /etc/rc.local
```

Also don't forget to remove `/home/mrtg/create-required-folders.sh` and `ExecStartPre` line from the service.
