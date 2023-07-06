## cmd

<!-- MarkdownTOC -->

- [Change font in console](#change-font-in-console)
- [CLI arguments](#cli-arguments)

<!-- /MarkdownTOC -->

### Change font in console

Say you'd like to set Consolas font in your cmd console. Usually you would click on window icon in top-left corner, go to settings, find fonts, but it only has a choice of two ugly fonts.

To add more fonts there, launch `regedit` and add `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Console\TrueTypeFont` with name `00` and key `Consolas` or any other (*the font must be available in the system*).

Reboot the system, and then this font will appear in the list of fonts to choose from. To add more fonts, new keys names should be `000`, `0000` and so on.

### CLI arguments

``` cmd
@echo off

set arg1=%1
set arg2=%2

echo first: %1 or %arg1%
echo second: %2 or %arg2%
echo all of them: %*
```
``` cmd
> .\some.bat ololo fuuuu other another stuff
first: ololo or ololo
second: fuuuu or fuuuu
all of them: ololo fuuuu other another stuff
```
