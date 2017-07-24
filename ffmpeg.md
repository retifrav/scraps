## FFmpeg

### Cut video fragment

``` bash
ffmpeg -i 1.mp4 -ss 00:03:05 -t 00:01:06 -vcodec copy -acodec copy cut.mp4
```

This will cut 66 seconds (00:01:06) from `1.mp4` starting from 00:03:05 timestamp and save it to `cup.mp4`.