# Synology DSM

<!-- MarkdownTOC -->

- [Remove a system package](#remove-a-system-package)
- [Issue a self-signed certificate](#issue-a-self-signed-certificate)
- [UPS server](#ups-server)

<!-- /MarkdownTOC -->

## Remove a system package

Such as Active Insight. Login with administrator via SSH (*or create a task to run on boot*) and execute:

``` sh
$ synopkg stop ActiveInsight
$ synopkg uninstall ActiveInsight
```

You can also list all the installed packages:

``` sh
$ /usr/syno/bin/synopkg list
```

## Issue a self-signed certificate

Specifically, for Synology DSM web-interface:

``` sh
$ openssl req -new -newkey rsa:4096 -x509 -sha256 -days 1111 -nodes \
    -out synology-certificate.crt -keyout synology-key.key
```

and then import it in `Control Panel` → `Security` → `Certificate` → `Add` → `Add` a new certificate (*or `Replace`*) → `Import certificate` and select private key and certificate files, no need for Intermediate certificate. All the browsers/client will then need to trust this new certificate too.

## UPS server

To expose USB-connected UPS data to other Synology NASes, Home Assistant [integration](https://home-assistant.io/integrations/nut/) or just CLI-based [NUT](https://networkupstools.org), open `Control Panel` → `Hardware & Power` → `UPS` and IP-addresses for the clients in `Permitted Synology NAS Devices`.

To test it from Mac OS:

``` sh
$ brew install nut

$ upsc ups@YOUR-MAIN-NAS-IP-ADDRESS:3493

battery.charge: 100
battery.charge.low: 10
battery.mfr.date: 2001/01/01
battery.runtime: 3312
battery.runtime.low: 120
battery.type: PbAc
battery.voltage: 28.5
battery.voltage.nominal: 24.0
device.mfr: American Power Conversion
device.model: Back-UPS BX1600MI
device.serial: SERIAL-NUMBER-HERE
device.type: ups
driver.name: usbhid-ups
driver.parameter.pollfreq: 30
driver.parameter.pollinterval: 5
driver.parameter.port: auto
driver.parameter.synchronous: no
driver.version: DSM-VERSION-HERE
driver.version.data: APC HID 0.96
driver.version.internal: 0.41
input.sensitivity: medium
input.transfer.high: 295
input.transfer.low: 145
input.voltage: 234.0
input.voltage.nominal: 230
ups.beeper.status: enabled
ups.delay.shutdown: 20
ups.firmware: 378600G -302202G 
ups.load: 0
ups.mfr: American Power Conversion
ups.mfr.date: 2023/11/16
ups.model: Back-UPS BX1600MI
ups.productid: 0002
ups.realpower.nominal: 900
ups.serial: SERIAL-NUMBER-HERE
ups.status: OL
ups.test.result: Done and passed
ups.timer.reboot: 0
ups.timer.shutdown: -1
ups.vendorid: 051d
```

In general, it seems to be using hardcoded credentials `monuser`/`secret`, but as you can see above, it works with just the `ups` value provided. And in Home Assistant integration it does not need even that, just leave the credentials fields empty. Note that if your Home Assistant is running in a Docker container (*on that same Synology NAS?*), then you probably will need to add that container IP address (*`172.x.x.x`*) to the list of permitted devices too.
