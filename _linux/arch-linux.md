# Arch Linux

<!-- MarkdownTOC -->

- [Get window ID](#get-window-id)
- [Reconnect network connection](#reconnect-network-connection)
- [File types associations](#file-types-associations)
    - [Get MIME type for a file](#get-mime-type-for-a-file)
    - [Default application for MIME type](#default-application-for-mime-type)
- [Applications](#applications)
    - [DaVinci Resolve](#davinci-resolve)

<!-- /MarkdownTOC -->

## Get window ID

``` sh
$ wmctrl -l
0x0160000b  7 towelie btop
0x01c0000b  0 towelie wmctrl -l
0x01e00004  1 towelie xorg - List windows on command line - Ask Ubuntu — Firefox Developer Edition
0x02000003  2 towelie ~/.config/i3/config - Sublime Text
0x02400003  3 towelie ~/code/some/thing - Sublime Merge
0x02600006  4 towelie some.kdbx [Locked] - KeePassXC
0x02a00008  5 towelie 1.FM - Amsterdam Trance Radio - strm112.1.fm - Blank & Jones - Mind of the wonderful (Radio Edit) (Feat. Elles de Graaf) - Audacious
0x03000009  6 towelie OBS 32.0.2 - Profile: Untitled - Scenes: Untitled
0x02e00002  7     N/A FreeRDP: ec2-26-36-112-67.eu-north-1.compute.amazonaws.com
0x03a00002  0     N/A Cowboy Bebop 01 - Asteroid Blues.mkv - mpv

$ xprop -id 0x03a00002
```

## Reconnect network connection

``` sh
$ nmcli con
NAME                UUID                                  TYPE      DEVICE 
Wired connection 1  42c2658a-ee63-39cd-9ee4-7c5c41562dd9  ethernet  enp3s0 
lo                  8f45dc54-eb6d-41a0-aaf1-cbc3c8485da0  loopback  lo     
some                c1ca92df-ed53-48c3-bec5-869c8460adcf  vpn       --

$ nmcli con down id "Wired connection 1"
Connection 'Wired connection 1' successfully deactivated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/4)
$ nmcli con up id "Wired connection 1"
Connection successfully activated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/7)
```

## File types associations

### Get MIME type for a file

``` sh
$ xdg-mime query filetype ./some.jpg 
image/jpeg
```

### Default application for MIME type

``` sh
$ xdg-mime default mupdf.desktop application/pdf
$ xdg-mime default feh.desktop image/jpeg

$ less ~/.config/mimeapps.list

$ XDG_UTILS_DEBUG_LEVEL=2 xdg-mime query default image/jpeg
Checking /home/vasya/.config/mimeapps.list
feh.desktop
```

## Applications

### DaVinci Resolve

``` sh
$ /opt/resolve/bin/resolve 
/opt/resolve/bin/resolve: symbol lookup error: /usr/lib/libpango-1.0.so.0: undefined symbol: g_once_init_leave_pointer

$ cd /opt/resolve/libs/
$ mkdir old
$ mv ./libglib* ./libgio* ./libgmodule* ./old/

$ cd /opt/resolve/bin/
$ mkdir ./BlackmagicRawAPI
$ mv ../libs/libBlackmagicRawAPI.so ./BlackmagicRawAPI/
```

But that's all pointless, as free version (*the one that is not Studio*) doesn't not support importing almost any videos.
