# FFmpeg

- [Cut video fragment](#cut-video-fragment)
- [Choose between audio tracks](#choose-between-audio-tracks)
- [Extract subtitles from container](#extract-subtitles-from-container)
- [Video encoding](#video-encoding)
- [Watermark on each frame](#watermark-on-each-frame)
- [Crop video](#crop-video)
- [Screen capture](#screen-capture)
- [Convert video to GIF](#convert-video-to-gif)
- [Convert video to Apple-compatible format](#convert-video-to-apple-compatible-format)
- [Blur specific region for a period of time](#blur-specific-region-for-a-period-of-time)

## Cut video fragment

``` bash
ffmpeg -i 1.mp4 -ss 00:03:05 -t 00:01:06 -vcodec copy -acodec copy cut.mp4
```
This will cut 66 seconds (00:01:06) from `1.mp4` starting from 00:03:05 timestamp and save it to `cup.mp4`.

## Choose between audio tracks

Get info about file:

``` bash
ffmpeg -i 1.mp4 -hide_banner
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

## Extract subtitles from container

Check the file's info and discover the subtitles track number. After that:

``` bash
ffmpeg -i 1.mkv -map 0:2 1.ass
```

## Video encoding

``` bash
ffmpeg.exe -i 1.avi -crf 18 out.mp4
```

* `-crf` - some kind of "level of quality" from `0` (best) to `51` (worst). Value `18` is "[visually lossless or nearly so](https://trac.ffmpeg.org/wiki/Encode/H.264#a1.ChooseaCRFvalue)".

## Watermark on each frame

``` bash
ffmpeg.exe -i 1.mp4 -vf "movie=logo.png [logo]; [in][logo] overlay=16:16[out]" -crf 18 2.mkv
```

Watermark file `logo.png` is in the same directory. Value `16:16` sets coordinates for top-left corner of watermark image.

## Crop video

Say, you have source file with 1366x768 and you want to crop 300 px:

``` bash
ffmpeg.exe -i 1.mp4 -filter:v "crop=1066:768:300:0" -crf 18 cut.mp4
```

First pair (`1066:768`) sets a new frame size, and second pair (`300:0`) sets coordinates for its top-left corver relatively from the original.

## Screen capture

First you need to install some capture device. For Windows it could be [Screen Capture Recorder](https://github.com/rdp/screen-capture-recorder-to-video-windows-free).

``` bash
ffmpeg.exe -f dshow -i audio="virtual-audio-capturer":video="screen-capture-recorder" -acodec pcm_s16le 
-vcodec libx264 -preset ultrafast -qp 0 testing.mkv
```

For more details read [my article](https://retifrav.github.io/blog/2017/04/24/record-the-screen-with-ffmpeg/) about screen recording with FFmpeg on Windows.

## Convert video to GIF

``` bash
ffmpeg.exe -i video.mov -pix_fmt rgb8 -r 15 -vf scale=700:-1 output.gif
```

* `-pix_fmt rgb8` - lowers the picture quality;
* `-r 15` - sets FPS to `15`;
* `-vf scale=700:-1` - sets the frame size.

## Convert video to Apple-compatible format

In order to convert your video file (even though it's already `mp4`) to an iMovie/QuickTime format (`yuv420p`):

``` bash
ffmpeg -i in.mp4 -pix_fmt yuv420p out.mp4
```

## Blur specific region for a period of time

We have a `1280x720` video and we want to blur some region like this (*in school I would get some punishment for placing sizes that way*):

![AppleScript mail message count](./ffmpeg-blurred-region.png?raw=true "FFmpeg, blur specifig region")

We need to apply a complex filter:

``` bash
ffmpeg -i some.mp4 -filter_complex \
 "[0:v]crop=490:500:790:220,boxblur=15[fg]; \
  [0:v][fg]overlay=790:220:enable='between(t,1,10)'[v]" \
-map "[v]" -map 0:a -crf 18 -movflags +faststart blurred.mp4
```

* `490:500` - width and height of the region we want to blur;
* `790:220` - **x** and **y** coordinates of the top-left corner for this region;
* `enable='between(t,1,10)'` - enables blurring only from 00:00:01 to 00:00:10 timestamps (9 seconds in total) of the video;
* `boxblur=15` - the strength of blurring.
