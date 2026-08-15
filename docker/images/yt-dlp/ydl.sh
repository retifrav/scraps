#!/bin/ash

# the LD_LIBRARY_PATH export that was here before is no longer needed:
# current deno base images isolate glibc in `/usr/local/lib/glibc`
# via `patchelf --set-rpath` plus an `ld.so.cache` instead of setting
# `LD_LIBRARY_PATH` and shadowing musl's default search path
#
# you can verify that it's all good with `docker run --rm --entrypoint ffmpeg <image> -version`

tmpfileOut=$(mktemp)
tmpfileLog=$(mktemp)

echo "[$(date +%Y-%m-%d-%H-%M-%S)] Requested a download of: $1" > "$tmpfileLog"
yt-dlp "$1" > /dev/null 2>>"$tmpfileOut"
echo "" >> "$tmpfileLog" && echo "---" >> "$tmpfileLog" && echo "" >> "$tmpfileLog"

cat "$tmpfileLog" >> ~/downloads/requested-urls.log && rm "$tmpfileLog"
cat "$tmpfileOut" > ~/downloads/yt-dlp-last-download.log && rm "$tmpfileOut"
