# Synology DSM

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
