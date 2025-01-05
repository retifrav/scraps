#!/bin/sh

tmpfile=$(mktemp)

echo "[$(date +%Y-%m-%d-%H-%M-%S)] Requested a download of: $1" > $tmpfile
yt-dlp $1 -P home:~/downloads/ 2>>$tmpfile
echo "" >> $tmpfile && echo "---" >> $tmpfile && echo "" >> $tmpfile

cat $tmpfile >> ~/downloads/requested-urls.log && rm $tmpfile
