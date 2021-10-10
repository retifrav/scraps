#!/bin/bash

DMG=$(dirname "$0")
DIR=/tmp/tnt$RANDOM
rm -rf $DIR
mkdir -p $DIR
cp "$DMG/Manual install"/*.dmg $DIR
xattr -r -d com.apple.quarantine $DIR/*.dmg  &>/dev/null
if [ $? -ne 0 ]; then
    echo "Failed to add a Gatekeeper exception"
    exit 1
fi
mkdir -p $DIR/mount
hdiutil attach -owners on -quiet -noverify -mountpoint $DIR/mount $DIR/*.dmg -shadow $DIR/shadow
find $DIR/mount -maxdepth 1 \! -type l \! -path $DIR/mount -exec xattr -r -d com.apple.quarantine {} \; &>/dev/null
(sleep 5 && hdiutil detach -force "$DMG") &
exit 0
