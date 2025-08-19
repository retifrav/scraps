## mpv

- <https://mpv.io/>
- <https://github.com/mpv-player/mpv>

### Windows

#### MSYS2 UCRT64

<https://github.com/mpv-player/mpv/blob/master/DOCS/compile-windows.md>

The environment is the UCRT64 variant of MSYS (*`ucrt64.exe`*). The packages must have the "correct" prefix in their names, such as `mingw-w64-ucrt-x86_64-*`. If you'll be using a different environment, make sure to change prefixes accordingly (*so for MinGW64 it would be `mingw-w64-x86_64-*`*).

``` sh
$ pacman -Syu
$ pacman -S \
    git \
    mingw-w64-ucrt-x86_64-toolchain \
    mingw-w64-ucrt-x86_64-meson \
    mingw-w64-ucrt-x86_64-libass \
    mingw-w64-ucrt-x86_64-luajit \
    mingw-w64-ucrt-x86_64-lcms2 \
    mingw-w64-ucrt-x86_64-glslang
```

The `mingw-w64-ucrt-x86_64-ffmpeg` and `mingw-w64-ucrt-x86_64-libplacebo` packages should also be required, but out of the box they are dynamically linked or something, and so static linking of the final mpv executable will fail with "*cannot find shaderc_shared/spirv-cross-c-shared/etc*", so instead you should build them (*statically linked*) from sources with Meson (*there will be instructions below*). Also, `mingw-w64-ucrt-x86_64-glslang` might not be actually needed, as mpv builds fine without it (*but probably with a limited functionality?*).

Check the executables:

``` sh
$ which git
/usr/bin/git
$ git --version
git version 2.50.1

$ which gcc
/ucrt64/bin/gcc
$ gcc --version
gcc.exe (Rev8, Built by MSYS2 project) 15.2.0

$ which python
/ucrt64/bin/python
$ python --version
Python 3.12.11

$ which meson
/ucrt64/bin/meson
$ meson --version
1.8.3
```

If you are getting this:

``` sh
$ meson --version
Traceback (most recent call last):
  File "<frozen runpy>", line 198, in _run_module_as_main
  File "<frozen runpy>", line 88, in _run_code
  File "E:/tools/msys2/ucrt64/bin/meson.exe/__main__.py", line 2, in <module>
  File "E:/tools/python/Lib/re/__init__.py", line 126, in <module>
    from . import _compiler, _parser
  File "E:/tools/python/Lib/re/_compiler.py", line 18, in <module>
    assert _sre.MAGIC == MAGIC, "SRE module mismatch"
           ^^^^^^^^^^^^^^^^^^^
AssertionError: SRE module mismatch
```

then it might be because your "host" Python from outside of MSYS environment gets in the way:

``` sh
$ echo $PYTHONPATH
E:\tools\python;E:\tools\python\Scripts;E:\tools\python\Lib

$ unset PYTHONPATH
```

Continue:

``` sh
$ cd /path/to/programs
$ mkdir mpv && cd $_
$ git clone --branch v0.40.0 --single-branch git@github.com:mpv-player/mpv.git src
$ cd ./src

$ mkdir ./subprojects

$ cat <<EOF > subprojects/ffmpeg.wrap
[wrap-git]
url = https://gitlab.freedesktop.org/gstreamer/meson-ports/ffmpeg.git
revision = meson-7.1
depth = 1
[provide]
libavcodec = libavcodec_dep
libavdevice = libavdevice_dep
libavfilter = libavfilter_dep
libavformat = libavformat_dep
libavutil = libavutil_dep
libswresample = libswresample_dep
libswscale = libswscale_dep
EOF

$ cat <<EOF > subprojects/libplacebo.wrap
[wrap-git]
url = https://github.com/haasn/libplacebo
revision = master
depth = 1
clone-recursive = true
EOF

$ meson setup \
    -Dgpl=true \
    -Ddefault_library=static -Dprefer_static=true -Dc_link_args='-static' -Dcpp_link_args='-static' \
    -Dlua=luajit \
    -Dlcms2=enabled -Dlibplacebo:lcms=enabled \
    build
```

The output might look like this:

``` sh
Build targets in project: 166

FFmpeg 7.1.1

  Basics
    License                                           : LGPL version 2.1 or later
    static                                            : YES
    shared                                            : NO
    postprocessing support                            : NO
    network support                                   : YES
    threading support                                 : w32threads
    safe bitstream reader                             : YES

  x86
    standalone assembly                               : YES
    x86 assembler                                     : nasm
    MMX                                               : YES
    MMXEXT                                            : YES
    3DNow!                                            : YES
    3DNow! extended                                   : YES
    SSE                                               : YES
    SSSE3                                             : YES
    AESNI                                             : YES
    AVX                                               : YES
    AVX2                                              : YES
    AVX-512                                           : YES
    AVX-512ICL                                        : YES
    XOP                                               : YES
    FMA3                                              : YES
    FMA4                                              : YES
    i686 features                                     : YES
    CMOV is fast                                      : YES
    EBX available                                     : YES
    EBP available                                     : NO

  Components
    External libraries                                : bzlib lzma mediafoundation schannel zlib
    External libraries providing hardware acceleration: d3d11va d3d12va dxva2
    Libraries                                         : avdevice avfilter swscale avformat avcodec swresample avutil
    Programs                                          :

libplacebo 7.352.0

  Optional features
    d3d11       : NO
    dovi        : YES
    gl-proc-addr: YES
    glslang     : YES
    lcms        : YES
    libdovi     : NO
    opengl      : YES
    shaderc     : NO
    vk-proc-addr: YES
    vulkan      : YES
    xxhash      : YES

mpv 0.40.0

    d3d11          : NO
    javascript     : NO
    libmpv         : YES
    lua            : YES
    opengl         : YES
    vulkan         : NO
    wayland        : NO
    x11            : NO

  Subprojects
    ffmpeg         : YES 8 warnings
    libplacebo     : YES 5 warnings
    nasm           : YES (from ffmpeg)

  User defined options
    c_link_args    : -static
    cpp_link_args  : -static
    default_library: static
    gpl            : true
    lcms2          : enabled
    libplacebo:lcms: enabled
    lua            : luajit
    prefer_static  : true

Found ninja-1.13.0 at D:\programs\msys2\ucrt64\bin/ninja.EXE
```

Now the build:

``` sh
$ ninja -C build mpv.exe mpv.com

$ echo $?
0

$ echo 'I have no stdout for it, something about shells integration, or I dunno'
$ ./build/mpv.exe --version > ./version.txt \
    && cat ./version.txt \
    && rm ./version.txt
```
