## systemd

<!-- MarkdownTOC -->

- [Create a new service](#create-a-new-service)
    - [System](#system)
    - [User](#user)
- [Status of the service](#status-of-the-service)
- [View log of the service](#view-log-of-the-service)
- [Limit service log size](#limit-service-log-size)
- [Restart the service](#restart-the-service)
- [Reload changed configuration](#reload-changed-configuration)
- [List all services and their status](#list-all-services-and-their-status)

<!-- /MarkdownTOC -->

### Create a new service

#### System

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

#### User

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

### Status of the service

System:

``` sh
$ systemctl status some.service
```

User:

``` sh
$ systemctl --user status some.service
```

### View log of the service

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

### Limit service log size

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

### Restart the service

System:

``` sh
$ sudo systemctl restart some.service
```

User:

``` sh
$ systemctl --user restart some.service
```

### Reload changed configuration

``` sh
$ sudo systemctl daemon-reload
```

### List all services and their status

``` sh
$ service --status-all
```
