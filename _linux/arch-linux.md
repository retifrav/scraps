# Arch Linux

<!-- MarkdownTOC -->

- [Get window ID](#get-window-id)
- [Reconnect network connection](#reconnect-network-connection)
- [File types associations](#file-types-associations)
    - [Get MIME type for a file](#get-mime-type-for-a-file)
    - [Default application for MIME type](#default-application-for-mime-type)
- [Docker](#docker)
    - [Changing path to Docker data](#changing-path-to-docker-data)
- [Locale](#locale)
- [Applications](#applications)
    - [DaVinci Resolve](#davinci-resolve)
    - [VS Code](#vs-code)
        - [Gigantic UI](#gigantic-ui)
        - [Keyring for secrets](#keyring-for-secrets)
        - [Jupyter kernel](#jupyter-kernel)

<!-- /MarkdownTOC -->

## Get window ID

``` sh
$ wmctrl -l
0x0160000b  7 towelie btop
0x01c0000b  0 towelie wmctrl -l
0x01e00004  1 towelie xorg - List windows on command line - Ask Ubuntu — Firefox Developer Edition
0x02000003  2 towelie ~/.config/i3/config - Sublime Text
0x02400003  3 towelie ~/code/some/thing - Sublime Merge
0x02600006  4 towelie some.kdbx [Locked] - KeePassXC
0x02a00008  5 towelie 1.FM - Amsterdam Trance Radio - strm112.1.fm - Blank & Jones - Mind of the wonderful (Radio Edit) (Feat. Elles de Graaf) - Audacious
0x03000009  6 towelie OBS 32.0.2 - Profile: Untitled - Scenes: Untitled
0x02e00002  7     N/A FreeRDP: ec2-26-36-112-67.eu-north-1.compute.amazonaws.com
0x03a00002  0     N/A Cowboy Bebop 01 - Asteroid Blues.mkv - mpv

$ xprop -id 0x03a00002
```

## Reconnect network connection

``` sh
$ nmcli con
NAME                UUID                                  TYPE      DEVICE 
Wired connection 1  42c2658a-ee63-39cd-9ee4-7c5c41562dd9  ethernet  enp3s0 
lo                  8f45dc54-eb6d-41a0-aaf1-cbc3c8485da0  loopback  lo     
some                c1ca92df-ed53-48c3-bec5-869c8460adcf  vpn       --

$ nmcli con down id "Wired connection 1"
Connection 'Wired connection 1' successfully deactivated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/4)
$ nmcli con up id "Wired connection 1"
Connection successfully activated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/7)
```

## File types associations

### Get MIME type for a file

``` sh
$ xdg-mime query filetype ./some.jpg 
image/jpeg
```

### Default application for MIME type

``` sh
$ xdg-mime default mupdf.desktop application/pdf
$ xdg-mime default feh.desktop image/jpeg

$ less ~/.config/mimeapps.list

$ XDG_UTILS_DEBUG_LEVEL=2 xdg-mime query default image/jpeg
Checking /home/vasya/.config/mimeapps.list
feh.desktop
```

## Docker

``` sh
$ sudo pacman -S docker docker-buildx
$ sudo systemctl enable docker.socket 
$ sudo systemctl start docker.socket 
$ sudo systemctl status docker.socket 

$ sudo docker info
Client:
 Version:    29.1.1
 Context:    default
 Debug Mode: false
 Plugins:
  buildx: Docker Buildx (Docker Inc.)
    Version:  0.30.1
    Path:     /usr/lib/docker/cli-plugins/docker-buildx
```

Instead of `docker.socket` you can enable and start `docker.service`, but then it will be loaded/started on every boot, while the `docker.socket` variant is [supposed](https://github.com/moby/moby/issues/38373#issuecomment-447393517) to be starting only when you actually launch something Docker-related.

### Changing path to Docker data

Optionally, you might want to move Docker stuff to a different location:

``` sh
$ sudo systemctl stop docker.socket
$ ls -l /data
$ sudo mv /var/lib/docker /data/

$ sudo mkdir /etc/docker
$ sudo nano /etc/docker/daemon.json
```
``` json
{
    "data-root": "/data/docker"
}
```
``` sh
$ sudo systemctl start docker.socket 
$ sudo docker images
```

but that won't do much good, as it uses `containerd`, so the actual(?) data will be in `/var/lib/containerd/`, and if you try to change that following the [official documentation](https://docs.docker.com/engine/daemon/#configure-the-data-directory-location), then it will fail for example like this:

``` sh
Error response from daemon: rpc error: code = NotFound desc = blob sha256:560c09b53106f2a9f45100bb105a5eb87ddb7d547f275caba7f37ab9b574a2fa expected at /var/lib/containerd/io.containerd.content.v1.content/blobs/sha256/560c09b53106f2a9f45100bb105a5eb87ddb7d547f275caba7f37ab9b574a2fa: blob not found: not found
```

## Locale

<https://wiki.archlinux.org/title/Locale>

``` sh
$ locale --all-locales
C
C.utf8
en_US.utf8
POSIX
ru_RU.utf8

$ sudo nano /etc/locale.gen
```
``` sh
# ...
en_GB.UTF-8 UTF-8
# ...
#en_US.UTF-8 UTF-8
# ...
```
``` sh
$ sudo locale-gen

$ locale --all-locales
locale: Cannot set LC_CTYPE to default locale: No such file or directory
locale: Cannot set LC_MESSAGES to default locale: No such file or directory
locale: Cannot set LC_COLLATE to default locale: No such file or directory
C
C.utf8
POSIX
en_GB.utf8
ru_RU.utf8

$ printenv | grep -E '^(LANG|LC_)'
LANG=en_US.UTF-8
LC_TIME=C.UTF-8

$ sudo nano /etc/locale.conf
```
``` sh
LANG=en_GB.UTF-8
LC_TIME=C.UTF-8
```
``` sh
$ sudo reboot
```
``` sh
$ printenv | grep -E '^(LANG|LC_)'
LANG=en_GB.UTF-8
LC_TIME=C.UTF-8

$ locale --all-locales
C
C.utf8
en_GB.utf8
POSIX
ru_RU.utf8
```

## Applications

### DaVinci Resolve

``` sh
$ /opt/resolve/bin/resolve 
/opt/resolve/bin/resolve: symbol lookup error: /usr/lib/libpango-1.0.so.0: undefined symbol: g_once_init_leave_pointer

$ cd /opt/resolve/libs/
$ mkdir old
$ mv ./libglib* ./libgio* ./libgmodule* ./old/

$ cd /opt/resolve/bin/
$ mkdir ./BlackmagicRawAPI
$ mv ../libs/libBlackmagicRawAPI.so ./BlackmagicRawAPI/
```

But that's all pointless, as free version (*the one that is not Studio*) doesn't not support importing almost any videos.

### VS Code

#### Gigantic UI

If you only need it to work when launching from console, then in `~/.vscode-oss/argv.json`:

``` json
{
    "enable-crash-reporter": false,
    "force-device-scale-factor": 2
}
```

And if you need it to work when launching from [Rofi](https://github.com/davatorium/rofi) or some other launcher, then in `~/.config/code-flags.conf` (*and remove it in `argv.json`*):

``` sh
--force-device-scale-factor=2
```

#### Keyring for secrets

Trying to log-in to GitHub from VS Code will fail without a system secrets storage, and one of those is this:

``` sh
$ sudo pacman -S gnome-keyring libsecret
```

Then in `~/.vscode-oss/argv.json` you need to specify `password-store`:

``` json
{
    "enable-crash-reporter": false,
    "password-store": "gnome-libsecret"
}
```

#### Jupyter kernel

If won't show neither kernels no virtual environments untill you add `enable-proposed-api` into `~/.vscode-oss/argv.json`:

``` json
{
    "enable-crash-reporter": false,
    "enable-proposed-api":
    [
        "ms-python.python",
        "ms-toolsai.jupyter"
    ]
}
```
