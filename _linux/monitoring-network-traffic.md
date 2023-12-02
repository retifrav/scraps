## Monitoring network traffic

<https://techrepublic.com/article/how-to-monitor-snmp-traffic-on-ubuntu-for-free-with-mrtg/>

<!-- MarkdownTOC -->

- [SNMP](#snmp)
- [MRTG](#mrtg)

<!-- /MarkdownTOC -->

### SNMP

``` sh
$ sudo apt install snmpd snmp
```

``` sh
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
```

``` sh
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

At first there will be just `index.html` there:

``` sh
$ ls -L1 /var/www/YOUR.HOST/admin/mrtg/
index.html
```

but a minute later or less there will be also other files:

``` sh
$ ls -L1 /var/www/YOUR.HOST/admin/mrtg/
index.html
localhost_ens3-day.png
localhost_ens3-month.png
localhost_ens3-week.png
localhost_ens3-year.png
localhost_ens3.html
localhost_ens3.log
mrtg-l.png
mrtg-m.png
mrtg-r.png
```

Make the files available to your web-server:

``` sh
$ sudo chown -R mrtg:www-data /var/www/YOUR.HOST/admin/mrtg
```

or/and add `mrtg` user to `www-data` group:

``` sh
$ sudo usermod -a -G www-data mrtg
```

And probably protect that route [with a password](https://github.com/retifrav/scraps/blob/master/_linux/index.md#basic-authentication).

The reports should be available at <https://YOUR.HOST/admin/mrtg/>.
