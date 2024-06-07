## NextDNS

A DNS provider with DNS-over-HTTPS and DNS-over-TLS, the website is [here](https://nextdns.io/?from=ykzu58c8) (*the link is referral*).

In general, it's just a matter of setting `45.90.28.0` and `45.90.30.0` as your DNS servers. But you can also [pass some more details](https://help.nextdns.io/t/x2h76ay/device-information-log-enrichment) for viewing logs in your profile.

<!-- MarkdownTOC -->

- [GNU/Linux](#gnulinux)
    - [Via systemd-resolved](#via-systemd-resolved)
- [Windows](#windows)

<!-- /MarkdownTOC -->

### GNU/Linux

#### Via systemd-resolved

```
$ sudo nano /etc/systemd/resolved.conf

[Resolve]
DNS=45.90.28.0#deviceName-profileID.dns.nextdns.io
DNS=2a07:a8c0::#deviceName-profileID.dns.nextdns.io
DNS=45.90.30.0#deviceName-profileID.dns.nextdns.io
DNS=2a07:a8c1::#deviceName-profileID.dns.nextdns.io
DNSOverTLS=yes

$ sudo systemctl daemon-reload
$ sudo systemctl restart systemd-resolved
```

### Windows

First, optionally, find two closest servers (*with the lowest latency*), either by pinging yourself:

``` cmd
> ping /n 6 ipv4-anexia-waw-1.edge.nextdns.io
> ping /n 6 ipv4-zepto-waw-1.edge.nextdns.io
> ping /n 6 ipv4-edis-vno-1.edge.nextdns.io
> ping /n 6 ipv4-anexia-prg-1.edge.nextdns.io
> ping /n 6 ipv4-vultr-ams-1.edge.nextdns.io
> ping /n 6 ipv4-anexia-osl-1.edge.nextdns.io
> ping /n 6 ipv4-anexia-cph-1.edge.nextdns.io
```

or by opening <https://ping.nextdns.io/>.

You should also be fine with the default `45.90.28.0` and `45.90.30.0`.

Then launch `cmd` as Administrator and add those two servers there:

``` cmd
> netsh dns add encryption server=95.179.134.211 dohtemplate=https://dns1.nextdns.io/profileID/deviceName autoupgrade=yes udpfallback=no
> netsh dns add encryption server=217.146.13.3 dohtemplate=https://dns2.nextdns.io/profileID/deviceName autoupgrade=yes udpfallback=no
```

Check what's added:

``` cmd
netsh dns show encryption

Encryption settings for 95.179.134.211
----------------------------------------------------------------------
DNS-over-HTTPS template     : https://dns1.nextdns.io/profileID/deviceName
Auto-upgrade                : yes
UDP-fallback                : no

Encryption settings for 217.146.13.3
----------------------------------------------------------------------
DNS-over-HTTPS template     : https://dns2.nextdns.io/profileID/deviceName
Auto-upgrade                : yes
UDP-fallback                : no

...
```

Finally, open Control Panel (*or this new retarded Settings GUI*) and add manual DNS servers for IPv4:

- preferred: `95.179.134.211`
- alternative: `217.146.13.3`

Now if you open <https://test.nextdns.io/> and if everything is correct, the result will be something like:

``` json
{
    "status": "ok",
    "protocol": "DOH",
    "profile": "SOME-PROFILE",
    "client": "SOME-IP",
    "srcIP": "ANOTHER-IP",
    "destIP": "YET-ANOTHER-IP",
    "anycast": false,
    "server": "anexia-waw-1",
    "clientName": "unknown-doh",
    "deviceName": "deviceName",
    "deviceID": "SOME-ID"
}
```

and if something isn't correct, then the result will be something like:

``` json
{
    "status": "unconfigured",
    "resolver": "SOME-DIFFERENT-IP",
    "srcIP": "ANOTHER-IP",
    "server": "anexia-waw-1"
}
```

Note that if you are using a VPN, such as one configured with OpenVPN client, then it may be using a different "virtual" network adapter and DNS settings for that one might be overridden on every connection, so you may have to install NextDNS desktop application to overcome this.
