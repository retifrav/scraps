# Mac OS SDK missing sysroot

After updating **Mac OS** / **XCode** / **Qt** you might get this error while trying to build your Qt-application:

``` bash
no such sysroot directory: '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk' [-Wmissing-sysroot]
```

or:

``` bash
fatal error: 'TargetConditionals.h' file not found
```

In my case I had everything working on **Mac OS 10.12**, but after updating to **Mac OS 10.13** (and **Xcode 9**) it started to give me this error.

At first, I thought the solution is to edit this file: `/path/to/Qt/5.9.1/clang_64/mkspecs/qdevice.pri` and replace

``` bash
QMAKE_MAC_SDK = macosx
```

with

``` bash
QMAKE_MAC_SDK = macosx10.13
```

After that (and relaunching **Qt Creator**) I was able to re-build my Qt projects.

But actually that wasn't the true source of the problem, as I've tried to put back `QMAKE_MAC_SDK = macosx` and it didn't cause any troubles with building.

As it turns out, what you actually need to do is to delete `.qmake.stash` file from *all* of your project build directories, because this file "cashes" the path to the "old" SDK:

``` bash
QMAKE_MAC_SDK.macosx.Path = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk
```

So, if you have 9000+ projects, then you need to delete 9000+ `.qmake.stash` files. That's something that Qt should do by itself, I would say.
