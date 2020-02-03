## Mac OS

- [Hotkeys](#hotkeys)
- [System info](#system-info)
  - [Mac OS version](#mac-os-version)
  - [CPU](#cpu)
  - [GPU](#gpu)
- [Homebrew](#homebrew)
  - [Search for package](#search-for-package)
  - [Install package](#install-package)
  - [List of installed packages](#list-of-installed-packages)
  - [Update](#update)
  - [Uninstall package](#uninstall-package)
  - [Cleanup](#cleanup)
- [Current date and time in UTC](#current-date-and-time-in-utc)
- [Resize pictures preserving aspect ratio](#resize-pictures-preserving-aspect-ratio)
- [Prevent Mac from sleeping](#prevent-mac-from-sleeping)
- [Get your IP address](#get-your-ip-address)
  - [Local](#local)
  - [External](#external)
- [Disable Gatekeeper](#disable-gatekeeper)
- [Search](#search)
  - [Discover the biggest files](#discover-the-biggest-files)
    - [Using sort](#using-sort)
    - [Using gsort](#using-gsort)
  - [Search in your folders](#search-in-your-folders)
  - [Looking for a string in files contents](#looking-for-a-string-in-files-contents)
- [Filter out error messages](#filter-out-error-messages)
- [ZIP files](#zip-files)
- [Create a dummy file to occupy space](#create-a-dummy-file-to-occupy-space)
- [Working with an SD card](#working-with-an-sd-card)
  - [Create an image of the card](#create-an-image-of-the-card)
  - [Write the image to the card](#write-the-image-to-the-card)
  - [Format the card](#format-the-card)
    - [FAT32](#fat32)
    - [JHFSX](#jhfsx)
- [Generate PPK file from RSA](#generate-ppk-file-from-rsa)
- [Convert OXPS to PDF](#convert-oxps-to-pdf)
- [Disable System Integrity Protection](#disable-system-integrity-protection)
- [Change system sounds](#change-system-sounds)
- [Unlock files in folder](#unlock-files-in-folder)
- [Turn the screen off](#turn-the-screen-off)
- [Connect to VPN](#connect-to-vpn)
- [Wi-Fi access point](#wi-fi-access-point)
- [Startup items paths](#startup-items-paths)
- [Get disk allocation block size](#get-disk-allocation-block-size)
- [Build a C++ program](#build-a-c-program)
- [Rebuild icons cache](#rebuild-icons-cache)
- [Calculate SHA checksums](#calculate-sha-checksums)
- [ImageMagick](#imagemagick)
- [Reset privacy settings for applications](#reset-privacy-settings-for-applications)

### Hotkeys

* ⌘ - command
* ⌥ - option
* ⌃ - control
* ⇧ - shift

Zoom parts of the screen:

* gradually zoom in:
  - ⌘ ⌥ +
* gradually zoom out:
  - ⌘ ⌥ -
* instantly zooms in to the last used scale or reset the zoom to default 100%:
  - ⌘ ⌥ 8

Minimize all windows:

* ⌘ ⌥ H M

Lock:

* ⌘ ⌃ Q

### System info

#### Mac OS version

```
sw_vers -productVersion
```

or

```
system_profiler SPSoftwareDataType
```

#### CPU

```
sysctl -n machdep.cpu.brand_string
```

#### GPU

```
system_profiler SPDisplaysDataType
```

### Homebrew

https://docs.brew.sh/FAQ

#### Search for package

```
brew search file retriever
```

#### Install package

```
brew install wget
```

#### List of installed packages

```
brew list
```

Only packages you installed, without dependencies:

```
brew leaves
```

Tree of dependencies:

```
brew deps --tree --installed
```

#### Update

```
brew update
brew outdated
brew upgrade
```

#### Uninstall package

```
brew uninstall wget
```

#### Cleanup

To remove old versions of packages.

List what can be cleaned up:

```
brew cleanup -n
```

Cleanup particular package:

```
brew cleanup wget
```

Cleanup everything:

```
brew cleanup
```

### Current date and time in UTC

`date '+%d.%m.%Y %H:%M:%S UTC%z'`

This will give something like this:

`09.03.2017 12:56:33 UTC+0100`

### Resize pictures preserving aspect ratio

```
sips --resampleWidth 800 -s formatOptions high *.jpg
```

which is the same thing as:

```
sips --resampleWidth 800 *.jpg
```

* `--resampleWidth 800` - resizes a picture to 800px width (preserving aspect ratio)
* `-s formatOptions high` - quality settings [`low`|`normal`|`high`|`best`|`100`]
* `*.jpg` - search mask for files that will be processed

### Prevent Mac from sleeping

```
caffeinate -u -t 600
```

* `-u` - emulates "user" usage
* `-t 600` - 600 seconds (10 minutes)

### Get your IP address

#### Local

```
ifconfig | grep "inet" | grep -Fv 127.0.0.1 | grep -Fv ::1 | awk '{print $2}'
```

or without IPv6 addresses (add space after `inet` and remove `::1` filter):

```
ifconfig | grep "inet " | grep -Fv 127.0.0.1 | awk '{print $2}'
```

or just:

```
ipconfig getifaddr en0
```

* `en0` - your network interface

#### External

```
curl ipecho.net/plain; echo
```

### Disable Gatekeeper

To allow installing applications from any source.

Check the status:

```
spctl --status
```

* `assessments enabled` - feature is turned on (you cannot install apps from any source)
* `assessments disabled` - feature is turned off (you can install from anywhere)

Turn off the feature:

```
sudo spctl --master-disable
```

Turn on the feature back:

```
sudo spctl --master-enable
```

### Search

#### Discover the biggest files

And the biggest directories too, of course.

##### Using sort

```
du -sh ~/* | sort -rn | head -10
```

* `du` - utility that displays the file system block usage
    * `-s` - show only top-level folders (without showing subfolders)
    * `-h` - present folders size in a human-readable format
    * `~/*` - count everything from the home directory
* `sort` - utility for sorting results
    * `-r` - reverse the list (because by default it goes ascending)
    * `-n` - sort by numeric values (size, in our case)
* `head` - shows only specified number of lines from result

However, this `sort` sorts only numbers without respecting the data unit (MB, GB, etc):

```
username@MacBook-Pro:~$ du -sh ~/* | sort -rn | head -10
880M	/Users/username/Applications
879M	/Users/username/temp
 56G	/Users/username/Library
 34M	/Users/username/Desktop
 23G	/Users/username/Pictures
 11G	/Users/username/Music
9.2G	/Users/username/Documents
```

##### Using gsort

```
du -sh ~/* | gsort -rh | head -10
```

```
username@some-MacBook-Pro:~$ du -sh ~/* | gsort -rh | head -10
 56G    /Users/username/Library
 23G    /Users/username/Pictures
 11G    /Users/username/Music
9.2G    /Users/username/Documents
6.3G    /Users/username/Movies
2.3G    /Users/username/Downloads
880M    /Users/username/Applications
879M    /Users/username/temp
```

So, it is all the same, but instead of `sort` we are using `gsort`, which supports `h` option (that respects human-readable data units). If you don't have `gsort` in your system, it can be installed via `brew install coreutils`.

#### Search in your folders

Let's find all the files (and folders) in your home folder that are related to the **GarageBand** application:

```
find ~ -iname "*garage*"
```

Two `*` wildcards will help to find any file (and folder) that contains `garage` in any part of its name.

You can also look for all `.mp4` files in your home directory:

```
find ~ -iname "*.mp4"
```

And here's a more complex example: look for all `.mp4` files in your home directory, then sort results by the file size and show only top 10 biggest ones:

```
find ~ -iname "*.mp4" -print0 | xargs -0 du -sh | gsort -rh | head -10
```

* `find` - utility for searching
    * `-iname` - look in file names, ignoring case and using `*` wildcard to specify the `.mp4` extension
    * `-print0` - adds a NULL character (instead of newline) after the name of each found file. This is needed for long file names with spaces and stuff
* `xargs` - takes result strings from the previous command and uses them as arguments for the next command in pipe (for `du` in our case)
    * `-0` - tells xargs to expect NULL characters as separators instead of spaces (that aligns with our `-print0` from `find`)
* `gsort` - read about it [here](#discover-the-biggest-files)

#### Looking for a string in files contents

Search in files of parent directory only (without going into subfolders):

```
grep -ils "sOmE tEXt" *.txt
```

* `grep` - utility for searching text
    * `-i` - ignoring case
    * `-l` - output only file names
    * `-s` - don't show warnings like `Is a directory`
    * `*.txt` - file name pattern, but works only with a single directory level (doesn't apply to subfolders)

Search in subfolders too:

```
grep -ilrn "sOmE tEXt" *
```

* `-r` - search recursively (in subfolders). Now there is no need in option `-s` (I guess)
* `-n` - add line numbers

Search in particular files only:

```
grep -ilr "sOmE tEXt" --include=*.{txt,mark*} *
```

* `--include=` - proper file name pattern that applies to all folder levels. This particular one will process only `.txt` and `.markdown` (all `.mark*` ones, to be precise) files

Another (clearer) way:

```
grep --include=\*.{cpp,h} -irn "/some/path/" -e "sOmE tEXt"
```

### Filter out error messages

Say you want to exclude error messages from some output.

If you want to exclude all the errors:

```
find / -iname "*.mp4" 2>/dev/null
```

If you want to exclude only specific errors:

```
find / -iname "*.mp4" 2>&1 | grep -v "Operation not permitted" | grep -v "Permission denied"
```

### ZIP files

```
zip -r9T archiveName.zip folderToArchive -x "*.DS_Store"
```

* `-r` - recursive, including all subfolders
* `-9` - compression level: from `0` (no compression) to `9` (maximum compression)
* `-T` - test archive integrity after finishing

To unpack the archive into current folder:

```
unzip archiveName.zip
```

### Create a dummy file to occupy space

```
dd if=/dev/random of=/tmp/stupidfile.crap bs=20m
```

This will start to create a file, "growing" it with 20 MB chunks of random trash. The process will never stop, so you'll need to break it with `⌃ + C`.

If you want to monitor the file's size in Terminal, install and run `watch` utility:

```
brew install watch
watch ls -alh /tmp/stupidfile.crap
```

### Working with an SD card

That actually works with external USB flash drives too, and of course not only `.img` but `.iso` as well.

#### Create an image of the card

```
diskutil list
sudo dd if=/dev/rYOUR-CARD of=/path/to/image.img bs=1m
```

#### Write the image to the card

```
diskutil list
diskutil unmountDisk /dev/YOUR-CARD
sudo dd if=/path/to/image.img of=/dev/rYOUR-CARD bs=1m
```
* `r` - raw, makes the writing faster

You can watch the progress by pressing `⌃ + T` combination.

After it's finished, eject the drive:

```
diskutil eject /dev/YOUR-USB-DRIVE
```

#### Format the card

Available file systems:

```
diskutil listFilesystems
```

##### FAT32

```
diskutil list
sudo diskutil eraseDisk FAT32 CARD-LABEL MBRFormat /dev/YOUR-CARD
```

##### JHFSX

Mac OS Extended (Case-sensitive, Journaled):

```
sudo diskutil eraseDisk jhfsx MAC /dev/YOUR-CARD
```

Source and additional information: http://gree2.github.io/mac/command/2015/06/27/mac-diskutil-command

### Generate PPK file from RSA

So you have your RSA key to connect to some server via SFTP. But suddenly you need to connect to this server from Windows using FileZilla, and it accepts only PPK files.

```
brew install putty
puttygen ~/.ssh/id_rsa_server -o server.ppk
```

### Convert OXPS to PDF

```
brew install ghostscript
/usr/local/Cellar/ghostscript/9.26/bin/gxps -sDEVICE=pdfwrite -sOutputFile=~/Desktop/output.pdf -dNOPAUSE some-file.oxps
```

### Disable System Integrity Protection

1. Reboot the Mac and hold down `Command + R` after you hear the startup chime, this will boot macOS into Recovery Mode;
2. Click the Utilities menu at the top of the screen instead and choose Terminal;
3. `csrutil disable`;
4. Reboot the Mac.

### Change system sounds

1. Disable [System Integrity Protection](#disable-system-integrity-protection);
2. Choose the sound file you want to change (`/System/Library/Components/CoreAudio.component/Contents/SharedSupport/SystemSounds/dock/drag to trash.aif`);
3. Replace it with a sound of your choice;
4. Enable System Integrity Protection back.

### Unlock files in folder

```
chflags -R nouchg /path/to/some/folder
```

* `-R` - recursive;
* `nouchg` - removes immutable flag.

### Turn the screen off

```
pmset displaysleepnow
```

### Connect to VPN

```
sudo openconnect https://vpn.your.domain/full --user=vasya
```

### Wi-Fi access point

```
sudo networksetup -createnetworkservice Loopback lo0
sudo networksetup -setmanual Loopback 172.20.42.42 255.255.255.255
```

And then enable **Internet Sharing** for **Loopback** interface in the **Sharing** settings.

### Startup items paths

```
/Library/StartupItems
/Library/LaunchDaemons
/Library/LaunchAgents
/System/Library/LaunchAgents
/System/Library/LaunchDaemons
```

And how to disable daemons:

```
sudo launchctl unload /Library/LaunchDaemons/com.some.helper.plist
sudo rm /Library/LaunchDaemons/com.some.helper.plist
```

### Get disk allocation block size

```
$ diskutil list

/dev/disk0 (internal):
   ...

/dev/disk1 (synthesized):
   ...

/dev/disk2 (external, physical):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:     FDisk_partition_scheme                        *2.0 TB     disk2
   1:               Windows_NTFS Samsung_T5              2.0 TB     disk2s1

$ diskutil info disk2s1

   Device Identifier:         disk2s1
   Device Node:               /dev/disk2s1
   ...

   Volume Name:               Samsung_T5
   Mounted:                   Yes
   Mount Point:               /Volumes/Samsung_T5

   Partition Type:            Windows_NTFS
   File System Personality:   ExFAT
   Type (Bundle):             exfat
   Name (User Visible):       ExFAT

   Disk Size:                 2.0 TB (2000396321280 Bytes) (exactly 3907024065 512-Byte-Units)
   Device Block Size:         512 Bytes

   ...
   Allocation Block Size:     131072 Bytes
```

* `Device Block Size` - physical block size (sector);
* `Allocation Block Size` - logical block size: 131072 bytes is 128 KB per block (131072 B / 1024 = 128 KB), so one such block takes 256 sectors on this disk (1 sector is 512 bytes).

### Build a C++ program

```
$ nano some.cpp
```

``` cpp
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "ololo\n";

    return 0;
}
```

```
$ clang++ some.cpp -o some
$ ./some
```

If you need C++11 standard, then:

```
clang++ -std=c++11 some.cpp -o some
```

### Rebuild icons cache

From [Clearing the Icon Services Cache in Mojave](https://mjtsai.com/blog/2019/02/01/clearing-the-icon-services-cache-in-mojave/).

As administrator:

```
sudo rm -rfv /Library/Caches/com.apple.iconservices.store
sudo find /private/var/folders/ \( -name com.apple.dock.iconcache -or -name com.apple.iconservices \) -exec rm -rfv {} \;
```

Then as your normal user:

```
killall Dock
```

### Calculate SHA checksums

By default it calculates SHA1:

```
shasum ~/Downloads/clonezilla-live-20191024-eoan-amd64.iso
```

You can specify SHA256 (or other):

```
shasum -a 256 ~/Downloads/clonezilla-live-20191024-eoan-amd64.iso
```

### ImageMagick

Get version and supported delegates:

```
$ magick -version
Version: ImageMagick 7.0.9-8 Q16 x86_64 2019-12-09 https://imagemagick.org
Features: Cipher DPC HDRI Modules OpenMP(3.1)
Delegates (built-in): bzlib freetype heic jng jp2 jpeg lcms ltdl lzma openexr png tiff webp xml zlib
```

List supported formats:

```
magick identify -list format
```

Resize an image to a specific width (100 px) while preserving the aspect:

```
magick convert input.jpg -resize 100 output.jpg
```

Resize all images in the current folder and append the `-thumb` suffix to files names:

```
for f in *; do magick convert "$f" -resize 100 "${f%.*}-thumb.${f##*.}"; done
```

### Reset privacy settings for applications

```
$ tccutil reset AppleEvents
$ tccutil reset SystemPolicyAllFiles
```
