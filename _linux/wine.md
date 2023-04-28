## Wine

<!-- MarkdownTOC -->

- [Installing](#installing)
- [Using](#using)

<!-- /MarkdownTOC -->

### Installing

<https://wiki.winehq.org/Ubuntu>

``` sh
$ lsb_release -a
No LSB modules are available.
Distributor ID: Neon
Description:    KDE neon 5.27
Release:        22.04
Codename:       jammy

$ sudo wget -NP /etc/apt/sources.list.d/ https://dl.winehq.org/wine-builds/ubuntu/dists/jammy/winehq-jammy.sources
$ sudo apt update
$ sudo apt install --install-recommends winehq-stable
```

If it fails and you are on KDE Neon, then:

``` sh
$ sudo apt install libpoppler-glib8:{i386,amd64}=22.02.0-2ubuntu0.1
$ sudo apt install --install-recommends winehq-stable
```

Check and configure:

``` sh
$ wine --version
$ winecfg
```

That will configure default prefix at `~/.wine`.

Try to run something default:

``` sh
$ wine notepad
```

### Using

To create more prefixes (*one per program/game*):

``` sh
$ mkdir wine-prefixes
$ export WINEPREFIX=~/wine-prefixes/thebat
$ echo $WINEPREFIX
$ winecfg
```

Don't forget that `WINEPREFIX` is exported only in the current session/tab. You'll need to set it again next time you'd like to run within the same prefix in a new session (*after system reboot, for example*).

Run the installer:

``` sh
$ wine ~/Downloads/The.Bat.Professional.v10.3.3.3.exe
```

After it's installed, you can launch the application:

``` sh
$ wine ~/wine-prefixes/thebat/drive_c/programs/thebat/TheBat64.exe
```
