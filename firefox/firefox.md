# Firefox

<!-- MarkdownTOC -->

- [Context menu customization](#context-menu-customization)
    - [How to enable](#how-to-enable)
    - [Editing](#editing)
        - [About namespace](#about-namespace)
    - [Menu entries IDs](#menu-entries-ids)
- [Browser Toolbox](#browser-toolbox)

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
/* main */
#context-navigation,
#context-sep-navigation,
#passwordmgr-items-separator,
#context-savepage,
#context-sep-sendpagetodevice,
#context-sendpagetodevice,
#context-sep-selectall,
#context-selectall,
#context-take-screenshot,
#spell-separator,

/* link */
#context-openlink,
#context-openlinkprivate,
#context-sep-open,
#context-bookmarklink,
#context-searchselect,
#context-savelink,
#context-sep-sendlinktodevice,
#context-sendlinktodevice,

/* image */
#context-sendimage,
#context-sep-setbackground,
#context-setDesktopBackground,
#context-copyimage-contents

/* text */
#context-print-selection,

/* tabs */
#context_duplicateTab,
#context_bookmarkTab,
#context_moveTabOptions,
#context_sendTabToDevice,
#context_selectAllTabs,
#context_closeTabOptions,
#context_undoCloseTab,
#context_closeTab,
#tabContextMenu > menuseparator

{ display: none !important }
```

#### About namespace

Some tutorials say that you need to add the namespace line before all the things

``` css
@namespace url("http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul");
```

But that is [no longer required](https://www.userchrome.org/adding-style-recipes-userchrome-css.html#namespaces), and actually can prevent some things from working.

### Menu entries IDs

Here are some menu entries [IDs](https://searchfox.org/mozilla-release/source/browser/base/content/browser-context.inc):

![](./context-main.png?raw=true "Firefox context menu, main")

![](./context-link.png?raw=true "Firefox context menu, link")

Better yet, you can enable [Browser Toolbox](#browser-toolbox) and inspect menu entries ID directly:

![](./browser-toolbox-inspect.jpg?raw=true "Firefox Browser Toolbox, inspect")

To prevent context menus from closing, [disable popup auto-hide](https://developer.mozilla.org/en-US/docs/Tools/Browser_Toolbox#debugging_popups).

## Browser Toolbox

[Browser Toolbox](https://developer.mozilla.org/en-US/docs/Tools/Browser_Toolbox) looks like regular [Web Developer Tools](https://developer.mozilla.org/en-US/docs/Tools), but allows to inspect the Firefox itself.

If at some point it stops launching, delete `chrome_debugger_profile` folder from your profile folder.
