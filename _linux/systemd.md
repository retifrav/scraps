# systemd

<!-- MarkdownTOC -->

- [Create a new service](#create-a-new-service)
    - [System](#system)
    - [User](#user)
- [Status](#status)
    - [List all services and their status](#list-all-services-and-their-status)
    - [Status of a service](#status-of-a-service)
    - [Restart a service](#restart-a-service)
- [Reload changed configuration](#reload-changed-configuration)
- [Logs](#logs)
    - [View log of the service](#view-log-of-the-service)
    - [Limit service log size](#limit-service-log-size)
- [Timers](#timers)

<!-- /MarkdownTOC -->

## Create a new service

### System

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

### User

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

## Status

### List all services and their status

``` sh
$ service --status-all
```

### Status of a service

System:

``` sh
$ systemctl status some.service
```

User:

``` sh
$ systemctl --user status some.service
```

### Restart a service

System:

``` sh
$ sudo systemctl restart some.service
```

User:

``` sh
$ systemctl --user restart some.service
```

## Reload changed configuration

After modifying a service:

``` sh
$ sudo systemctl daemon-reload
```

## Logs

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

## Timers

Some script that does something:

``` sh
$ mkdir ~/scripts
$ nano ~/scripts/do-something.sh

$ chmod +x ~/scripts/do-something.sh
$ ~/scripts/do-something.sh
$ echo $?
```

Service for it:

``` sh
$ mkdir -p ~/.config/systemd/user/

$ nano ~/.config/systemd/user/do-something.service
```
``` ini
[Unit]
Description=Do something

[Service]
Type=oneshot
ExecStart=/home/deck/scripts/do-something.sh
```

Timer for it:

``` sh
$ nano ~/.config/systemd/user/do-something.timer
```
``` ini
[Unit]
Description=Do something every minute

[Timer]
OnCalendar=*-*-* *:*:00
AccuracySec=1s
Persistent=true

[Install]
WantedBy=timers.target
```

Enable that timer (*no need to enable the service?*):

``` sh
$ systemctl --user daemon-reload
$ systemctl --user enable --now do-something.timer

$ systemctl --user list-timers
NEXT                         LEFT LAST                         PASSED UNIT                   ACTIVATES
Sun 2026-06-07 19:10:00 CEST  51s Sun 2026-06-07 19:09:00 CEST 7s ago do-something.timer do-something.service

$ journalctl --user -u do-something.service
```
