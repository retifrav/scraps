# Synology DSM

<!-- MarkdownTOC -->

- [Remove a system package](#remove-a-system-package)
- [Issue a self-signed certificate](#issue-a-self-signed-certificate)

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
