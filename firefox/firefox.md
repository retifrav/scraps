# Firefox

<!-- MarkdownTOC -->

- [Context menu customization](#context-menu-customization)
    - [How to enable](#how-to-enable)
    - [Editing](#editing)
        - [About namespace](#about-namespace)

<!-- /MarkdownTOC -->

## Context menu customization

### How to enable

1. Open `about:config`, set `toolkit.legacyUserProfileCustomizations.stylesheets` to `true`.
2. Open `about:profiles`, find your current profile, open its Root Directory (`C:\Users\YOUR-NAME\AppData\Roaming\Mozilla\Firefox\Profiles\l3s7hjbi.some`)
3. Create folder `chrome`, and inside it create file `userChrome.css`. That's where you will be customizing stuff

Better to backup your `userChrome.css` from time to time, as Firefox can override/delete it on next updates.

### Editing

Open `userChrome.css` and disable things you don't want in your context menus. For example:

``` css
#context-navigation,
#context-sep-navigation,

#context-savepage,
#context-sep-sendpagetodevice,
#context-sendpagetodevice,
#context-selectall,

#context-openlink,
#context-sendlinktodevice

{ display: none !important }
```

Some menu entries IDs:

![](./context-main.png?raw=true "Firefox context menu, main")

![](./context-link.png?raw=true "Firefox context menu, link")

More IDs: https://searchfox.org/mozilla-release/source/browser/base/content/browser-context.inc

#### About namespace

Some tutorial say that you need to add the namespace line before all the things

``` css
@namespace url("http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul");
```

But that is [no longer required](https://www.userchrome.org/adding-style-recipes-userchrome-css.html#namespaces), and actually can prevent some things from working.
