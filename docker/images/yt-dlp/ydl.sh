#!/bin/ash

# when `screen` is in detached mode, it does not get environment variables,
# so they need to be exported again (or maybe do `source /etc/profile` instead)
export LD_LIBRARY_PATH=/usr/lib:/usr/local/lib

tmpfileOut=$(mktemp)
tmpfileLog=$(mktemp)

echo "[$(date +%Y-%m-%d-%H-%M-%S)] Requested a download of: $1" > $tmpfileLog
yt-dlp $1 > /dev/null 2>>$tmpfileOut
echo "" >> $tmpfileLog && echo "---" >> $tmpfileLog && echo "" >> $tmpfileLog

cat $tmpfileLog >> ~/downloads/requested-urls.log && rm $tmpfileLog
cat $tmpfileOut > ~/downloads/yt-dlp-last-download.log && rm $tmpfileOut
