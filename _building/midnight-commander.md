# Midnight Commander

Building [Midnight Commander](https://midnight-commander.org) from sources.

<!-- MarkdownTOC -->

- [Local prefix](#local-prefix)
- [Building](#building)
- [Slow start](#slow-start)

<!-- /MarkdownTOC -->

## Local prefix

A situation when you are on a GNU/Linux host without root, so you can only build and install stuff within your home folder. In my case it was a Red Hat Enterprise Linux Server release 7.7 with, although with [Lmod](https://lmod.readthedocs.io/en/latest/010_user.html) and [EasyBuild](https://docs.easybuild.io), but not everything was available in those, as it turned out.

Suppose, you have your local prefix in `~/.local/`, then export `PKG_CONFIG_PATH` in your `~/.bashrc`:

``` sh
export PKG_CONFIG_PATH="$HOME/.local/lib/pkgconfig:$PKG_CONFIG_PATH"
```

## Building

Download sources and try to configure:

``` sh
$ mkdir ~/sources && cd $_
$ wget https://github.com/MidnightCommander/mc/archive/refs/tags/4.8.30.tar.gz
$ tar -xvf ./mc-4.8.30.tar.gz
$ rm ./mc-4.8.30.tar.gz
$ mv ./mc-4.8.30 ./mignight-commander
$ cd ./mignight-commander && mkdir build && cd $_
$ ./autogen.sh
$ cd ./build/

$ ../configure --prefix=/home/YOUR-USERNAME/.local
checking for GLIB... no
configure: error: glib-2.0 not found or version too old (must be >= 2.30)
```

Luckly, this one was available through EasyBuild:

``` sh
$ eb --search-filename glib
$ eb GLib-2.75.0-GCCcore-12.2.0.eb --fetch
$ eb GLib-2.75.0-GCCcore-12.2.0.eb --robot --parallel 10
$ module load GLib/2.75.0-GCCcore-12.2.0
$ module save mine
```

Next problem on configure:

``` sh
$ ../configure --prefix=/home/YOUR-USERNAME/.local
checking for SLANG... no
configure: error: S-Lang >= 2.0.0 library not found
```

This one isn't available via EasyBuild, so have to build it from sources:

``` sh
$ cd ~/sources
$ wget https://www.jedsoft.org/snapshots/slang-pre2.3.4-8.tar.gz
$ tar -xvf ./slang-pre2.3.4-8.tar.gz
$ rm ./slang-pre2.3.4-8.tar.gz
$ mv ./slang-pre2.3.4-8 ./slang
$ cd ./slang
$ echo "It doesn't support shadow build"
$ ./configure --prefix=/home/YOUR-USERNAME/.local
$ time make -j10
$ make install
```

Then configuration will succeed:

``` sh
$ cd ~/sources/mignight-commander/build/
$ ../configure --prefix=/home/YOUR-USERNAME/.local
```

But build will fail:

``` sh
$ time make -j10
/cluster/software/binutils/2.39-GCCcore-12.2.0/bin/ld: cannot find -ltermcap: No such file or directory
collect2: error: ld returned 1 exit status
```

Because one more dependency is missing:

``` sh
$ cd ~/sources
$ wget ftp://ftp.gnu.org//gnu/termcap/termcap-1.3.1.tar.gz
$ tar -xvf ./termcap-1.3.1.tar.gz
$ rm ./termcap-1.3.1.tar.gz
$ mv ./termcap-1.3.1 ./termcap
$ cd ./termcap && mkdir build && cd $_
$ ../configure --prefix=/home/YOUR-USERNAME/.local
$ make -j10
gcc -c  -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DSTDC_HEADERS=1  -DTERMCAP_FILE=\"/etc/termcap\" -I. -I.. -g ../version.c
../tparam.c: In function 'memory_out':
../tparam.c:51:3: warning: implicit declaration of function 'write' [-Wimplicit-function-declaration]
   51 |   write (2, "virtual memory exhausted\n", 25);
      |   ^~~~~
as: unrecognized option '--gdwarf-5'
make: *** [version.o] Error 1
```

I don't know what exactly that is, but it seems to be somehow related to Fortran, because after I loaded its module, the error went away:

``` sh
$ module avail fortran
$ module load netCDF-Fortran/4.6.0-gompi-2022b
$ module list
$ module save mine

$ cd ~/sources/termcap/build/
$ rm r ./*; ls -lah
$ ../configure --prefix=/home/YOUR-USERNAME/.local
$ make -j10
$ make install
```

It will output several installation errors, as for some reason it still tries to install some of the files into the system prefix instead of the one provided with `--prefix` on configuration, but most importantly the library will be installed correctly. And then Midnight Commander build succeeds:

``` sh
$ cd ./mignight-commander/build
$ rm r ./*; ls -lah
$ ../configure --prefix=/home/YOUR-USERNAME/.local
$ make -j10
$ make install

$ mc --version
GNU Midnight Commander unknown
Built with GLib 2.75.0
Built with S-Lang pre2.3.4-8 with terminfo database
With builtin Editor
With subshell support as default
With support for background operations
With mouse support on xterm
With internationalization support
With multiple codepages support
Virtual File Systems:
 cpiofs, tarfs, sfs, extfs, ftpfs, fish
Data types:
 char: 8; int: 32; long: 64; void *: 64; size_t: 64; off_t: 64;
```

## Slow start

One issue though is that its starting time is long, like 10 seconds. You can "fix" that by disabling its sub-shell with an alias:

``` sh
alias mc="mc --nosubshell"
```

or, as the problem seems to be caused by failed hostname resolution:

```
export HOSTALIASES=$HOME/.hosts
```

and then in `~/.hosts`:

```
127.0.0.1 localhost login-1.betzy.sigma2.no login-2.betzy.sigma2.no betzy.sigma2.no
```

but that didn't work on this Red Hat distribution.
