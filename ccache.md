# ccache

Compiler cache - <https://ccache.dev/> - caches object files and does not do re-compilation is the sources haven't changed.

<!-- MarkdownTOC -->

- [Common](#common)
- [Linux](#linux)
    - [Arch Linux](#arch-linux)
- [Mac OS](#mac-os)
- [Windows](#windows)
    - [How Qt Creator handles it](#how-qt-creator-handles-it)
- [Using in-project variables instead of environment](#using-in-project-variables-instead-of-environment)

<!-- /MarkdownTOC -->

## Common

Once configured, if you'd like to disable it via environment variable for a particular session:

``` sh
$ export CCACHE_DISABLE=1
```

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

``` sh
$ brew install ccache

$ mkdir ~/.cache/ccache
$ mkdir ~/.config/ccache
$ nano ~/.config/ccache/ccache.conf
```
``` sh
cache_dir = /Users/USERNAME/.cache/ccache
max_size = 20.0G
```

``` sh
$ nano ~/.bash_profile
```
``` sh
# ...

# ccache
export CCACHE_CONFIGPATH="~/.config/ccache/ccache.conf"
export PATH="/opt/homebrew/opt/ccache/libexec:$PATH"
```

## Windows

Download [archive](https://ccache.dev/download.html), unpack somewhere, add that location to `PATH` in Environment Variables via Windows Advanced System Settings GUI. While there, also add `CCACHE_CONFIGPATH` environment variable with `C:\Users\USERNAME\.config\ccache\ccache.conf` value.

Create that `C:\Users\USERNAME\.config\ccache\ccache.conf` file with the following contents:

``` sh
cache_dir = c:/Users/USERNAME/.cache/ccache
max_size = 20.0G
```

and create that `c:/Users/USERNAME/.cache/ccache` folder.

Open a new shell and check that it picked up the config, so you get 20 GB instead of default 5 GB:

``` cmd
> ccache -s
Local storage:
  Cache size (GB): 0.0 / 20.0 ( 0.00%)
```

Now to enable it in a project you could try to add the following (*before or after the `project()` statement?*):

``` cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Windows") # other platforms have an easier way of enabling ccache
    find_program(CCACHE_EXECUTABLE ccache)
    if(CCACHE_EXECUTABLE)
        message(STATUS "Found ccache: ${CCACHE_EXECUTABLE}")
        set(CMAKE_C_COMPILER_LAUNCHER
            "${CCACHE_EXECUTABLE}"
            #CACHE STRING "C compiler launcher"
            #FORCE
        )
        set(CMAKE_CXX_COMPILER_LAUNCHER
            "${CCACHE_EXECUTABLE}"
            #CACHE STRING "CXX compiler launcher"
            #FORCE
        )
    else()
        message(WARNING "Could not find ccache")
    endif()
endif()
```

or, if that doesn't work, then you could try to enable it globally by setting `CMAKE_C_COMPILER_LAUNCHER`
and `CMAKE_CXX_COMPILER_LAUNCHER` environment variables, and apparently those need to be set to an absolute path to `ccache.exe` (*such as `D:\programs\ccache\ccache.exe`*). Actually, this seems to be a preferred way (*which actually works*), so there is no need to modify the project files.

If you are using vcpkg, then you might need to pass-through those variables in the triplet(s):

``` cmake
set(VCPKG_ENV_PASSTHROUGH_UNTRACKED
    # this one might have already been here
    VCPKG_ROOT
    # these are for ccache
    CCACHE_CONFIGPATH
    CMAKE_C_COMPILER_LAUNCHER
    CMAKE_CXX_COMPILER_LAUNCHER
)
```

It might also work if you set those cache variables in the (*chainloaded*) toolchain, but having the environment variables set seems to be enough.

Either way, all that apparently only works with Ninja generator, which is no worries, because who in a sane state of mind uses Visual Studio generators.

### How Qt Creator handles it

- <https://forum.qt.io/topic/147301/using-ccache-with-qt-creator-cmake-and-msvc>
- <https://code.qt.io/cgit/qt-creator/qt-creator.git/tree/cmake/QtCreatorAPIInternal.cmake#n124>

Apparently, `/Zi` vs `/Z7` flags might be an issue, so:

``` cmake
# SCCACHE does not work with the /Zi option, which makes each compilation write debug info
# into the same .pdb file - even with /FS, which usually makes this work in the first place.
# Replace /Zi with /Z7, which leaves the debug info in the object files until link time.
# This increases memory usage, disk space usage and linking time, so should only be
# enabled if necessary.
# Must be called after project(...).
function(qtc_handle_compiler_cache_support)
  if (WITH_SCCACHE_SUPPORT OR WITH_CCACHE_SUPPORT)
    if (MSVC)
      foreach(config DEBUG RELWITHDEBINFO)
        foreach(lang C CXX)
          set(flags_var "CMAKE_${lang}_FLAGS_${config}")
          string(REPLACE "/Zi" "/Z7" ${flags_var} "${${flags_var}}")
          set(${flags_var} "${${flags_var}}" PARENT_SCOPE)
        endforeach()
      endforeach()
    endif()
  endif()
  if (WITH_CCACHE_SUPPORT)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
      set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" CACHE STRING "CXX compiler launcher" FORCE)
      set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" CACHE STRING "C compiler launcher" FORCE)
    endif()
  endif()
endfunction()
```

## Using in-project variables instead of environment

The above instructions seem to work with Ninja only(?), and to enable `ccache` for other generators (*Visual Studio and Xcode*) you most likely will need to do that within the project by setting certain CMake variables.

Craig Scott has this covered in detail in his [Professional CMake](https://crascit.com/professional-cmake/) book, there is even an entire `Appendix A` with a full example (*which I won't copy-paste here, so do buy the book*). There is also a less detailed but similar example [here](https://github.com/TheLartians/Ccache.cmake/tree/master).

I'd reckon this method would be challenging to apply to vcpkg-managed builds, but fortunately those builds are already handled with Ninja, so environment variables should suffice.
