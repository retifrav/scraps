# Arch Linux

<!-- MarkdownTOC -->

- [Packages](#packages)
    - [Downgrading a package](#downgrading-a-package)
- [Locale](#locale)
- [Display](#display)
    - [Screen tearing](#screen-tearing)
    - [Flickering of GUI applications with NVIDIA, i3 and picom](#flickering-of-gui-applications-with-nvidia-i3-and-picom)
- [Get window ID](#get-window-id)
- [Screenshots](#screenshots)
    - [maim](#maim)
- [Network](#network)
    - [Reconnect network connection](#reconnect-network-connection)
    - [VPN and RDP](#vpn-and-rdp)
        - [Disabling Kerberos DNS lookup](#disabling-kerberos-dns-lookup)
- [File types associations](#file-types-associations)
    - [Get MIME type for a file](#get-mime-type-for-a-file)
    - [Default application for MIME type](#default-application-for-mime-type)
- [Docker](#docker)
    - [Changing path to Docker data](#changing-path-to-docker-data)
    - [Claude Code](#claude-code)
- [TeX](#tex)
- [Applications](#applications)
    - [DaVinci Resolve](#davinci-resolve)
    - [VS Code](#vs-code)
        - [Gigantic UI](#gigantic-ui)
        - [Keyring for secrets](#keyring-for-secrets)
        - [Jupyter kernel](#jupyter-kernel)
    - [LibreOffice](#libreoffice)

<!-- /MarkdownTOC -->

## Packages

Package manager is [pacman](/_linux/pacman.md).

### Downgrading a package

<https://wiki.archlinux.org/title/Downgrading_packages#Using_the_pacman_cache>

``` sh
$ pacman -Q openvpn
openvpn 2.7.3-1

$ ls -L1 /var/cache/pacman/pkg/openvpn*.zst
/var/cache/pacman/pkg/openvpn-2.6.16-1-x86_64.pkg.tar.zst
/var/cache/pacman/pkg/openvpn-2.6.17-1-x86_64.pkg.tar.zst
/var/cache/pacman/pkg/openvpn-2.6.18-1-x86_64.pkg.tar.zst
/var/cache/pacman/pkg/openvpn-2.6.19-1-x86_64.pkg.tar.zst
/var/cache/pacman/pkg/openvpn-2.7.0-1-x86_64.pkg.tar.zst
/var/cache/pacman/pkg/openvpn-2.7.1-1-x86_64.pkg.tar.zst
/var/cache/pacman/pkg/openvpn-2.7.2-1-x86_64.pkg.tar.zst
/var/cache/pacman/pkg/openvpn-2.7.3-1-x86_64.pkg.tar.zst

$ sudo pacman -U file:///var/cache/pacman/pkg/openvpn-2.7.0-1-x86_64.pkg.tar.zst
warning: downgrading package openvpn (2.7.3-1 => 2.7.0-1)
resolving dependencies...
looking for conflicting packages...

Packages (1) openvpn-2.7.0-1

Total Installed Size:   1.73 MiB
Net Upgrade Size:      -0.01 MiB

$ pacman -Q openvpn
openvpn 2.7.0-1

$ sudo micro /etc/pacman.conf
```
``` ini
IgnorePkg = openvpn
```

If you need to ignore more than one package, then list them separated with spaces (*or add more `IgnorePkg` lines*):

``` ini
IgnorePkg = openvpn freerdp
```

Ignored packages will be ignored on running updates:

``` sh
$ sudo pacman -Suy
# ...
:: Starting full system upgrade...
warning: freerdp: ignoring package upgrade (2:3.20.0-1 => 2:3.25.0-1)
warning: openvpn: ignoring package upgrade (2.6.17-1 => 2.7.3-1)
```

If you don't have desired older packages in your local cache, you can download them from the [archive](https://archive.archlinux.org/packages/o/openvpn/).

Regarding `freerdp` and `openvpn` specifically, before actually downgrading, check [this Kerberos thing](#disabling-kerberos-dns-lookup) first.

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

## Display

### Screen tearing

If you are getting screen tearing on scrolling pages in web-browser or elsewhere, then you might need a compositor such as [picom](https://wiki.archlinux.org/title/Picom). To auto-start it in `~/.config/i3/config`:

``` sh
exec --no-startup-id picom -b
```

And in its config `~/.config/picom.conf`:

``` sh
# ...

shadow = false;
fading = false;
frame-opacity = 1.0;
vsync = true;
backend = "glx";
use-damage = false;
xrender-sync-fence = false;

# ...
```

### Flickering of GUI applications with NVIDIA, i3 and picom

<https://wiki.archlinux.org/title/NVIDIA/Troubleshooting#Avoid_screen_tearing>

Some GUI applications (*FileZilla, Sublime Text, etc*) might have flickering/flashing/tearing, and that seems to be a bug or something, which is apparently resolved with:

``` sh
$ sudo pacman -S nvidia-settings
$ nvidia-settings --assign CurrentMetaMode="nvidia-auto-select +0+0 { ForceFullCompositionPipeline = On }"
```

That is a temporary setting, just to verify that it helps at all. If it does, to make it permanent you'll need to [auto-generate](https://wiki.archlinux.org/title/NVIDIA#Automatic_configuration) NVIDIA settings and copy them to X11 config:

``` sh
$ echo 'Just in case, check for existing `/etc/X11/xorg.conf` first'

$ sudo nvidia-xconfig
$ less /etc/X11/xorg.conf
$ sudo mv /etc/X11/xorg.conf /etc/X11/xorg.conf.d/20-nvidia.conf
$ sudo nano /etc/X11/xorg.conf.d/20-nvidia.conf
```
``` sh
Section "Device"
    Identifier     "Device0"
    Driver         "nvidia"
    VendorName     "NVIDIA Corporation"
EndSection

Section "Screen"
    Identifier     "Screen0"
    Device         "Device0"
    Monitor        "Monitor0"
    Option         "ForceFullCompositionPipeline" "on"
    Option         "AllowIndirectGLXProtocol" "off"
    Option         "TripleBuffer" "on"
EndSection

Section "Screen"
    Identifier     "Screen0"
    Device         "Device0"
    Monitor        "Monitor1"
    Option         "ForceFullCompositionPipeline" "on"
    Option         "AllowIndirectGLXProtocol" "off"
    Option         "TripleBuffer" "on"
EndSection
```

There will be more sections in the auto-generated config, but only `Device` and `Screen` are needed. The `Monitor0` and `Monitor1` assume that you have 2 displays, but I have no idea where to get those identifiers.

Anyway, when this won't work, just enable `ForceFullCompositionPipeline` for both displays by running NVIDIA GUI with `nvidia-settings` and also set the main display to sync to:

``` sh
$ nvidia-settings
$ less ~/.nvidia-settings-rc
$ nvidia-settings --load-config-only
$ nvidia-settings --query CurrentMetaMode

  Attribute 'CurrentMetaMode' (towelie:0.0): id=50, switchable=no, source=nv-control :: DPY-2: nvidia-auto-select @3840x2160 +1920+0 {ViewPortIn=3840x2160,
  ViewPortOut=3840x2160+0+0, ForceCompositionPipeline=On, ForceFullCompositionPipeline=On}, DPY-0: nvidia-auto-select @1920x1080 +0+0 {ViewPortIn=1920x1080,
  ViewPortOut=1920x1080+0+0, ForceCompositionPipeline=On, ForceFullCompositionPipeline=On}
```

And if that helped, then add `nvidia-settings --load-config-only` to `~/.config/i3/config` for auto-starting.

When nothing helps, disable the fucking picom shit from auto-starting (*and kill it with fire*), as it doesn't seem to be really needed anyway (*even though [i3](https://wiki.archlinux.org/title/I3) does not provide compositing*), but you will probably get some [tearing](#screen-tearing) on scrolling pages. But at least it proves that picom is the one who's causing troubles.

And just in case revert all the changes you made for NVIDIA including those in `/etc/X11/xorg.conf.d/20-nvidia.conf`.

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

## Screenshots

### maim

<https://wiki.archlinux.org/title/Screen_capture#maim>

``` sh
$ sudo pacman -S maim xdotool
```

Show a selection rectangle and save a screenshot of the selected area to the specified file:

``` sh
$ maim --select ~/Downloads/1.png
```

Save a screenshot of the active window to the specified file (*to be invoked somehow not from the console*):

``` sh
$ maim --window $(xdotool getactivewindow) ~/Downloads/1.png
```

## Network

### Reconnect network connection

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

### VPN and RDP

#### Disabling Kerberos DNS lookup

You your RDP connection fails after a long series of something like:

``` sh
[ERROR][com.winpr.sspi.Kerberos] - [krb_log_context_encryption]: fn (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.winpr.sspi.Kerberos] - [krb5glue_get_init_creds]: krb5_init_creds_get (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.winpr.sspi.Kerberos] - [kerberos_AcquireCredentialsHandleA]: krb5glue_get_init_creds (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.freerdp.core.transport] - [transport_default_write]: BIO_should_retry returned an error: error:80000068:system library::Connection reset by peer
[ERROR][com.freerdp.core] - [transport_default_write]: ERRCONNECT_CONNECT_TRANSPORT_FAILED [0x0002000D]
[ERROR][com.freerdp.core.transport] - [transport_connect_nla]: NLA begin failed
[INFO][com.freerdp.codec] - [libavcodec_init]: Using VAAPI for accelerated H264 decoding
[INFO][com.freerdp.codec] - [libavcodec_init]: Using VAAPI for accelerated H264 decoding
[ERROR][com.winpr.sspi.Kerberos] - [krb_log_context_encryption]: fn (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.winpr.sspi.Kerberos] - [krb5glue_get_init_creds]: krb5_init_creds_get (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.winpr.sspi.Kerberos] - [kerberos_AcquireCredentialsHandleA]: krb5glue_get_init_creds (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.winpr.sspi.Kerberos] - [krb_log_context_encryption]: fn (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.winpr.sspi.Kerberos] - [krb5glue_get_init_creds]: krb5_init_creds_get (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.winpr.sspi.Kerberos] - [kerberos_AcquireCredentialsHandleA]: krb5glue_get_init_creds (Cannot find KDC for realm "YOUR-PC-HOSTNAME" [-1765328230])
[ERROR][com.freerdp.core.transport] - [transport_default_write]: BIO_should_retry returned an error: error:80000068:system library::Connection reset by peer
[ERROR][com.freerdp.core] - [transport_default_write]: ERRCONNECT_CONNECT_TRANSPORT_FAILED [0x0002000D]
[ERROR][com.freerdp.core.transport] - [transport_connect_nla]: NLA begin failed
[ERROR][com.freerdp.core] - [freerdp_connect]: freerdp_post_connect failed
```

Try disabling DNS lookup:

``` sh
$ sudo micro /etc/krb5.conf
```
``` ini
[libdefaults]
    # ...
    dns_lookup_kdc = false
    dns_lookup_realm = false
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

### Claude Code

The `Dockerfile`:

``` docker
FROM debian:trixie-slim

# without pipefail the `curl ... | bash` below would silently succeed when the download fails
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# ARG, not ENV - keeps DEBIAN_FRONTEND out of the runtime environment
ARG DEBIAN_FRONTEND=noninteractive

ENV TZ=Europe/Amsterdam \
    LANG=C.UTF-8 \
    DOTNET_CLI_TELEMETRY_OPTOUT=1 \
    DOTNET_NOLOGO=1

# base tooling first, because it changes least often
#
# the base image purges `/var/cache/apt` itself (`/etc/apt/apt.conf.d/docker-clean`),
# so `apt-get clean` is redundant, only need to remove package lists
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        bind9-dnsutils \
        build-essential \
        ca-certificates \
        chromium-headless-shell \
        cmake \
        curl \
        file \
        fontconfig \
        fonts-dejavu-core \
        fonts-liberation \
        git \
        git-lfs \
        gpg \
        jq \
        less \
        netcat-openbsd \
        openssh-client \
        openssl \
        p7zip-full \
        patch \
        pipx \
        postgresql-client \
        python3 \
        python3-venv \
        ripgrep \
        sqlite3 \
        tree \
        unzip \
        wget \
        zip \
    && rm -rf /var/lib/apt/lists/*

# the host directory mounted at `/home/claude` must be owned by `1234:1234`,
# otherwise Claude Code won't be able to write its own config
ARG UID=1234
ARG GID=1234
RUN groupadd -g ${GID} claude \
    && useradd -u ${UID} -g ${GID} -m -s /bin/bash claude \
    && mkdir -p /workspace \
    && chown claude:claude /workspace

# Claude Code is installed the last, because it changes most often
#
# the installer has no `--prefix`, but it is entirely $HOME-relative and writes
# an absolute `bin` symlink, so pointing HOME at the install prefix is enough,
# no moving or symlink rewriting required afterwards. It needs to stay out of
# `/home/claude` because that path is a bind mount at runtime, so anything installed
# there would be masked
#
# you can set `CLAUDE_VERSION` to `stable`, `latest` or an exact `x.y.z`
ARG CLAUDE_PREFIX=/opt/claude
ARG CLAUDE_VERSION=""
RUN mkdir -p "${CLAUDE_PREFIX}" \
    && curl -fsSL https://claude.ai/install.sh \
        | HOME="${CLAUDE_PREFIX}" bash -s -- ${CLAUDE_VERSION} \
    && ln -s "${CLAUDE_PREFIX}/.local/bin/claude" /usr/local/bin/claude \
    && rm -rf "${CLAUDE_PREFIX}"/.claude "${CLAUDE_PREFIX}"/.claude.json \
        "${CLAUDE_PREFIX}"/.cache "${CLAUDE_PREFIX}"/.npm \
    && HOME=/tmp claude --version \
    && rm -rf /tmp/.claude /tmp/.claude.json /tmp/.cache

USER claude

# ~/.local/bin is where pipx puts its shims, it lives in the mounted home, so it is appended
# instead of being prepended, so the image's tooling should win over whatever has accumulated
# in the persistent mapped home path
ENV PATH="${PATH}:/home/claude/.local/bin"

# https://code.claude.com/docs/en/data-usage#telemetry-services
ENV DISABLE_AUTOUPDATER=1 \
    DISABLE_ERROR_REPORTING=1 \
    DISABLE_TELEMETRY=1

WORKDIR /workspace

ENTRYPOINT ["claude"]
```

Building an image:

``` sh
$ sudo docker build . -f ./Dockerfile \
    -t claude:$(date +%Y-%m-%d-%H%M%S) \
    -t claude:latest
$ sudo dive claude
```

Folders (*with rootless Docker the [UID](/docker/index.md#uidgid-mapping) will be not `1234` but `101233`*):

``` sh
$ mkdir -p /data/claude/{.claude,workspace}
$ id
uid=1000(vasya) gid=1000(vasya) groups=1000(vasya)
$ sudo chown -R 1234:vasya /data/claude
```

Creating and running a container:

``` sh
$ docker run --init -it --rm \
    --name claude \
    -v "/data/claude/home:/home/claude" \
    -v "$(pwd):/workspace" \
    claude
```

With native installer, because of the crutches to move it out of `~/.local/`, it will be spamming warnings, but it seems to work fine otherwise.

## TeX

<https://wiki.archlinux.org/title/TeX_Live>

``` sh
$ cd ~/Downloads/
$ wget https://mirror.ctan.org/systems/texlive/tlnet/install-tl-unx.tar.gz
$ tar -xvf ./install-tl-unx.tar.gz && rm ./install-tl-unx.tar.gz
$ cd ./install-tl-20260503/

$ mkdir -p ~/programs/texlive/2026

$ which perl
$ perl ./install-tl
```

It will launch a TUI installer, where you will need to change the base path to `~/programs/texlive/2026/` (*look at the paths it wants and adjust accordingly*).

You might also want to uncheck some packages, such as languages that you will never use.

After the installation is done, it will instruct you to set the `PATH` and other environment variables, which you can do in `~/.bashrc`:

``` sh
# TeX
export INFOPATH="/home/vasya/programs/texlive/2026/texmf-dist/doc/info:$INFOPATH"
export MANPATH="/home/vasya/programs/texlive/2026/texmf-dist/doc/man:$MANPATH"
export PATH="/home/vasya/programs/texlive/2026/bin/x86_64-linux:$PATH"
```

To verify that it all works, open a new shell and:

``` sh
$ cd /tmp && mkdir ./tx && cd $_
$ pdflatex small2e
$  ls -L1
small2e.aux
small2e.log
small2e.pdf
$ mupdf ./small2e.pdf
```

Updating:

``` sh
$ tlmgr update --list
$ tlmgr update --all
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
$ sudo pacman -S gnome-keyring libsecret seahorse
```

Launch `seahorse` and make sure that you have a keyring marked as default and that you know the password for it, otherwise re-create it.

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

### LibreOffice

<https://wiki.archlinux.org/title/LibreOffice>

The package and fonts:

``` sh
$ sudo pacman -S libreoffice-fresh \
    ttf-caladea \
    ttf-carlito \
    ttf-dejavu \
    ttf-liberation \
    ttf-linux-libertine-g \
    noto-fonts \
    adobe-source-code-pro-fonts \
    adobe-source-sans-fonts \
    adobe-source-serif-fonts
```

Disable splash screen:

``` sh
$ sudo micro /etc/libreoffice/sofficerc
```
``` ini
Logo=0
```
