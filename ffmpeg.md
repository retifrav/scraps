## FFmpeg

- [Cut video fragment](#cut-video-fragment)
- [Choose between audio tracks](#choose-between-audio-tracks)
- [Extract subtitles from container](#extract-subtitles-from-container)
- [Video encoding](#video-encoding)

### Cut video fragment

``` bash
ffmpeg -i 1.mp4 -ss 00:03:05 -t 00:01:06 -vcodec copy -acodec copy cut.mp4
```
This will cut 66 seconds (00:01:06) from `1.mp4` starting from 00:03:05 timestamp and save it to `cup.mp4`.

### Choose between audio tracks

Get info about file:

``` bash
ffmpeg 1.mp4
```

Find info about audio tracks:

``` bash
Duration: 01:52:52.80, start: 0.000000, bitrate: 2768 kb/s 
  Stream #0:0: Video: mpeg4 (XVID / 0x44495658), yuv420p, 704x384 [SAR 1:1 DAR 11:6]
  Stream #0:1: Audio: ac3 ([0] [0][0] / 0x2000), 48000 Hz, 5.1(side), s16, 448 kb/s
  Stream #0:2: Audio: ac3 ([0] [0][0] / 0x2000), 48000 Hz, 5.1(side), s16, 448 kb/s
```

So, we want 63 seconds of video and second audio track:

``` bash
ffmpeg -i 1.mkv -map 0:0 -map 0:2 -ss 01:37:34 -t 63 -vcodec copy -acodec copy cut.mkv
```

### Extract subtitles from container

Check the file's info and discover the subtitles track number. After that:

``` bash
ffmpeg -i 1.mkv -map 0:2 1.ass
```

### Video encoding

``` bash
ffmpeg.exe -i 1.avi -crf 18 out.mp4
```

* `CRF` - some kind of "level of quality" from `0` (best) to `51` (worst). Value `18` is "practically equal to the source".
