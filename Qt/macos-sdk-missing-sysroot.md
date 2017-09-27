## Mac OS SDK missing sysroot

After updating **Mac OS** / **XCode** / **Qt** you might get this error while trying to build your Qt-application:

``` bash
no such sysroot directory: '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk' [-Wmissing-sysroot]
```

or:

``` bash
fatal error: 'TargetConditionals.h' file not found
```

In my case I had everything working on **Mac OS 10.12**, but after updating to **Mac OS 10.13** (and **Xcode 9**) it started to give me this error.

The solution is to edit this file: `/path/to/Qt/5.9.1/clang_64/mkspecs/qdevice.pri` and replace

``` bash
QMAKE_MAC_SDK = macosx
```

with

``` bash
QMAKE_MAC_SDK = macosx10.13
```

And in case if your SDK version is different, then just use yours.

After that (and relaunching **Qt Creator**) my Qt projects are successfully built again.

And by the way, it will continue to work fine even if you will put back `QMAKE_MAC_SDK = macosx`. Apparently, it's just some one-time thing to set up some paths.
