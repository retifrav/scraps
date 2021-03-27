## qmake

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
