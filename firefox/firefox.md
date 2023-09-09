## Firefox

<!-- MarkdownTOC -->

- [Context menu customization](#context-menu-customization)
	- [How to enable](#how-to-enable)
	- [Editing](#editing)
		- [About namespace](#about-namespace)
	- [Menu entries IDs](#menu-entries-ids)
- [Browser Toolbox](#browser-toolbox)
- [Make a screenshot](#make-a-screenshot)
- [Trust certificates](#trust-certificates)
- [Export cookies](#export-cookies)

<!-- /MarkdownTOC -->

### Context menu customization

Based on [Guide How To Edit Your Context Menu](https://old.reddit.com/r/firefox/comments/7dvtw0/guide_how_to_edit_your_context_menu/).

#### How to enable

1. Open `about:config`, set `toolkit.legacyUserProfileCustomizations.stylesheets` to `true`.
2. Open `about:profiles`, find your current profile, open its Root Directory (`C:\Users\YOUR-NAME\AppData\Roaming\Mozilla\Firefox\Profiles\l3s7hjbi.some`)
3. Create folder `chrome`, and inside it create file `userChrome.css`. That's where you will be customizing stuff

Better to backup your `userChrome.css` from time to time, as Firefox can override/delete it on next updates.

#### Editing

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

##### About namespace

Some tutorials say that you need to add the namespace line before all the things:

``` css
@namespace url("http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul");
```

But that is [no longer required](https://www.userchrome.org/adding-style-recipes-userchrome-css.html#namespaces), and actually can prevent some things from working.

#### Menu entries IDs

Here are some menu entries [IDs](https://searchfox.org/mozilla-release/source/browser/base/content/browser-context.inc):

![](./context-main.png?raw=true "Firefox context menu, main")

![](./context-link.png?raw=true "Firefox context menu, link")

Better yet, you can enable [Browser Toolbox](#browser-toolbox) and inspect menu entries IDs directly:

![](./browser-toolbox-inspect.jpg?raw=true "Firefox Browser Toolbox, inspect")

To prevent context menus from closing, [disable popup auto-hide](https://developer.mozilla.org/en-US/docs/Tools/Browser_Toolbox#debugging_popups).

## Browser Toolbox

[Browser Toolbox](https://developer.mozilla.org/en-US/docs/Tools/Browser_Toolbox) looks like regular [Web Developer Tools](https://developer.mozilla.org/en-US/docs/Tools), but allows to inspect the Firefox itself.

If at some point it stops launching, delete `chrome_debugger_profile` folder from your profile folder.

### Make a screenshot

```
:screenshot --selector "#NODE-ID" --dpr 1
```

### Trust certificates

If you have some certificate in the system that you have added yourself, for instance in order to debug your web-requests via proxy, Firefox won't trust it, so you need to set this value in `about:config`:

```
security.enterprise_roots.enabled | true
```

### Export cookies

Exit Firefox, copy cookies database file to somewhere:

``` sh
$ cp ~/Library/Application\ Support/Firefox/Profiles/YOUR-PROFILE-ID/cookies.sqlite ~/Desktop/
```

Open it in SQLite and execute exporting query:

``` sh
$ sqlite3 -separator $'\t' ./cookies.sqlite
```
``` sql
.mode tabs
.header off

.once cookies.txt

SELECT
    host,
    CASE SUBSTR(host,1,1)='.' WHEN 0 THEN 'FALSE' ELSE 'TRUE' END,
    path,
    CASE isSecure WHEN 0 THEN 'FALSE' ELSE 'TRUE' END,
    expiry,
    name,
    value
FROM moz_cookies;

.exit
```

If you want to export cookies only for a specific domain, for example `*.nrk.no`, add `WHERE host LIKE '%nrk.no'`.

After the query results are written to the file, open it and add this on the first line, followed by an empty line to separate from the cookies:

```
# Netscape HTTP Cookie File

www.nrk.no	FALSE	/	FALSE	2797409944	_nrkbucket1	0
...
```
