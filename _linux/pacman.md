# pacman

<https://wiki.archlinux.org/title/Pacman>

A package manager on Arch Linux (*and SteamOS*) but also in MSYS2 on Windows.

<!-- MarkdownTOC -->

- [Search for a package](#search-for-a-package)
    - [Not installed](#not-installed)
    - [Installed](#installed)
- [Installing and uninstalling](#installing-and-uninstalling)
    - [Install a package](#install-a-package)
    - [Uninstall a package](#uninstall-a-package)
    - [Update all the installed packages](#update-all-the-installed-packages)
- [Inspecting installed packages](#inspecting-installed-packages)
    - [General information](#general-information)
    - [Find out which package owns the file](#find-out-which-package-owns-the-file)

<!-- /MarkdownTOC -->

## Search for a package

### Not installed

By package name and description:

``` sh
$ pacman -Ss xcb
```

or with a regular expression to search only by the package name:

``` sh
$ pacman -Ss '^xcb-.*'
```

Get information about the package:

``` sh
$ pacman -Si libxcb
```

### Installed

All the installed packages:

``` sh
$ pacman -Qs
```

Search among the installed packages:

``` sh
$ pacman -Qs zlib
```

## Installing and uninstalling

### Install a package

``` sh
$ sudo pacman -S libxcb
```

### Uninstall a package

``` sh
$ sudo pacman -Runs libxcb
```

here:

- `-R` - remove;
- `-u` - avoid removing packages if other packages depend on it(?);
- `-n` - not creating some backup `.pacsave` files;
- `-s` - remove dependencies too, if they are not needed for any other package.

### Update all the installed packages

``` sh
$ sudo pacman -Syu
```

## Inspecting installed packages

### General information

``` sh
$ pacman -Qi zlib
Name            : zlib
Version         : 1.3-1
Description     : Compression library implementing the deflate compression method found in gzip and PKZIP
Architecture    : x86_64
URL             : https://www.zlib.net/
Licenses        : custom
Groups          : libraries
Provides        : None
Depends On      : gcc-libs
Optional Deps   : None
Required By     : bsdtar  curl  file  gnupg  libcurl  libgnutls  libssh2  libxml2  wget
Optional For    : None
Conflicts With  : None
Replaces        : None
Installed Size  : 89.52 KiB
Packager        : CI (msys2/msys2-autobuild/0852421d/6008461504)
Build Date      : Tue Aug 29 07:58:22 2023
Install Date    : Fri Dec 27 20:04:52 2024
Install Reason  : Installed as a dependency for another package
Install Script  : No
Validated By    : Signature
```

Installed files:

``` sh
$ pacman -Ql zlib
zlib /usr/
zlib /usr/bin/
zlib /usr/bin/msys-z.dll
zlib /usr/share/
zlib /usr/share/licenses/
zlib /usr/share/licenses/zlib/
zlib /usr/share/licenses/zlib/LICENSE
zlib /usr/share/man/
zlib /usr/share/man/man3/
zlib /usr/share/man/man3/zlib.3.gz
```

### Find out which package owns the file

``` sh
$ pacman -Qo /usr/include/X11/Xlib.h
/usr/include/X11/Xlib.h is owned by libx11 1.1.3-6
```
