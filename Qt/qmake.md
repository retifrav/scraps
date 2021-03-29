## qmake

<!-- MarkdownTOC -->

- [Copy folders to application bundle on Mac OS](#copy-folders-to-application-bundle-on-mac-os)
- [Provide some parameters via CLI](#provide-some-parameters-via-cli)

<!-- /MarkdownTOC -->

### Copy folders to application bundle on Mac OS

Using Mac OS [scope](https://doc.qt.io/qt-5/qmake-language.html#scopes-and-conditions) block and [QMAKE_BUNDLE_DATA](https://doc.qt.io/qt-5/qmake-variable-reference.html#qmake-bundle-data) variable:

``` qmake
macx {
    # ...

    dplQSS.files = $$PWD/qss
    dplQSS.path = Contents/Resources
    QMAKE_BUNDLE_DATA += dplQSS

    dplChatStyles.files = \
        $$PWD/gui/qss/chat/Bubble \
        $$PWD/gui/qss/chat/Bubble_Compact
    dplChatStyles.path = Contents/Resources/stylesheets
    QMAKE_BUNDLE_DATA += dplChatStyles

    # ...
}
```

### Provide some parameters via CLI

For example, [building RetroShare](https://github.com/RetroShare/RetroShare/issues/2389#issuecomment-805757623) on Mac OS:

``` sh
$ ~/programs/qt/bin/qmake \
INCLUDEPATH+="/usr/local/opt/openssl/include" \
QMAKE_LIBDIR="/usr/local/opt/openssl/lib" \
QMAKE_LIBDIR+="/usr/local/opt/sqlcipher/lib" \
QMAKE_LIBDIR+="/usr/local/opt/miniupnpc/lib" \
..
```
