# ccache

<https://ccache.dev/>

<!-- MarkdownTOC -->

- [Linux](#linux)
    - [Arch Linux](#arch-linux)
- [Mac OS](#mac-os)
- [Windows](#windows)

<!-- /MarkdownTOC -->

## Linux

### Arch Linux

<https://wiki.archlinux.org/title/Ccache>

``` sh
$ sudo pacman -S ccache

$ mkdir -p /data/ccache
$ mkdir -p ~/.config/ccache
$ nano ~/.config/ccache/ccache.conf
```
``` sh
# might want to place it in `~/.cache/ccache` instead
cache_dir = /data/ccache
max_size = 20.0G
#sloppiness = locale,time_macros
```

``` sh
$ sudo pacman -S colorgcc
$ sudo nano /etc/colorgcc/colorgccrc
```
``` sh
# ...

#g++: /usr/bin/g++
#gcc: /usr/bin/gcc
#c++: /usr/bin/c++
#cc:  /usr/bin/cc
g++:  /usr/lib/ccache/bin/g++
gcc:  /usr/lib/ccache/bin/gcc
c++:  /usr/lib/ccache/bin/g++
cc:   /usr/lib/ccache/bin/cc

# ...
```

``` sh
$ nano ~/.bashrc
```
``` sh
# ...

# ccache (and colorgcc)
export PATH="/usr/lib/colorgcc/bin/:$PATH"
export PATH="/usr/lib/ccache/bin:$PATH"
# only use compilers here (because of colorgcc)
export CCACHE_PATH="/usr/bin"
```

## Mac OS

## Windows
