## FFmpeg

- [Cut video fragment](#cut-video-fragment)
- [Choose between audio tracks](#choose-between-audio-tracks)
- [Extract subtitles from container](#extract-subtitles-from-container)
- [Video encoding](#video-encoding)
- [Resize video](#resize-video)
- [Concat several video files](#concat-several-video-files)
  - [Concat a lot of video files](#concat-a-lot-of-video-files)
- [Watermark on each frame](#watermark-on-each-frame)
- [Crop video](#crop-video)
- [Screen capture](#screen-capture)
  - [GDI](#gdi)
  - [DirectShow](#directshow)
- [Convert video to GIF](#convert-video-to-gif)
- [Convert video to Apple-compatible format](#convert-video-to-apple-compatible-format)
- [Blur specific region for a period of time](#blur-specific-region-for-a-period-of-time)
- [Convert FLAC to ALAC](#convert-flac-to-alac)
  - [Mac OS / Linux](#mac-os--linux)
  - [Windows](#windows)
- [Sync video and audio](#sync-video-and-audio)
- [Add audio to video](#add-audio-to-video)
- [Slow or speed up the video](#slow-or-speed-up-the-video)
- [Rotate the video](#rotate-the-video)
- [Apply several filters at once](#apply-several-filters-at-once)
- [Make a video from images](#make-a-video-from-images)
- [ARF to MP4](#arf-to-mp4)

### Cut video fragment

``` bash
ffmpeg -ss 00:03:05 -i 1.mp4 -t 00:01:06 -vcodec copy -acodec copy cut.mp4
```
This will cut 66 seconds (00:01:06) from `1.mp4` starting from 00:03:05 timestamp and save it to `cup.mp4`. Putting `-ss 00:03:05` before `-i` [makes it](https://stackoverflow.com/a/33188399/) to start cutting from the nearest keyframe, so you won't have frozen frames or shit in your output video.

### Choose between audio tracks

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

### Extract subtitles from container

Check the file's info and discover the subtitles track number. After that:

``` bash
ffmpeg -i 1.mkv -map 0:2 1.ass
```

### Video encoding

``` bash
ffmpeg.exe -i 1.avi -crf 18 out.mp4
```

* `-crf` - some kind of "level of quality" from `0` (best) to `51` (worst). Value `18` is "[visually lossless or nearly so](https://trac.ffmpeg.org/wiki/Encode/H.264#a1.ChooseaCRFvalue)".

### Resize video

To make it smaller (frame dimensions), mostly.

``` bash
ffmpeg -i some.mov -vf scale=1022:-1 -crf 18 output.mp4
```

* `-vf scale=1022:-1` - output video will have `1022` width, and height value will be calculated correspondingly. It is possible to pick *wrong* values, and then FFmpeg will tell you something like `Error while opening encoder for output stream #0:0 - maybe incorrect parameters such as bit_rate, rate, width or height` or `height not divisible by 2` - simply adjust `scale` value a bit.

### Concat several video files

Concat 2 videos:

```
ffmpeg -i first.mp4 -i second.mp4 -filter_complex "[0:v] [0:a] [1:v] [1:a] concat=n=2:v=1:a=1 [v] [a]" -map "[v]" -map "[a]" out.mp4
```

Or maybe 3 videos:

```
ffmpeg -i first.mp4 -i second.mp4 -i third.mp4 -filter_complex "[0:v] [0:a] [1:v] [1:a] [2:v] [2:a] concat=n=3:v=1:a=1 [v] [a]" -map "[v]" -map "[a]" out.mp4
```

#### Concat a lot of video files

```
ls -1v *.mp4 > files.txt
```

On Mac OS use `gls`.

Edit the file:

```
file '1.mp4'
file '2.mp4'
file '3.mp4'
file '4.mp4'
file '5.mp4'
```

Concat:

```
ffmpeg -f concat -safe 0 -i files.txt output.mp4
```

If you need to download a YouTube playlist, which has videos in different formats, then download them like this:

```
youtube-dl -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]/mp4' --recode-video mp4 https://www.youtube.com/playlist?list=PLAYLIST-ID -o '%(autonumber)s-%(title)s'
```

### Watermark on each frame

``` bash
ffmpeg.exe -i 1.mp4 -vf "movie=logo.png [logo]; [in][logo] overlay=16:16[out]" -crf 18 2.mkv
```

Watermark file `logo.png` is in the same directory. Value `16:16` sets coordinates for top-left corner of watermark image.

### Crop video

Say, you have source file with 1366x768 and you want to crop 300 px:

``` bash
ffmpeg.exe -i 1.mp4 -filter:v "crop=1066:768:300:0" -crf 18 cut.mp4
```

First pair (`1066:768`) sets a new frame size, and second pair (`300:0`) sets coordinates for its top-left corner relatively from the original.

### Screen capture

#### GDI

```
ffmpeg.exe -f gdigrab -i desktop out.mp4
```

If you can boost the encoding with NVIDIA hardware acceleration:

```
ffmpeg.exe -f gdigrab -i desktop -c:v h264_nvenc -qp 0 out.mp4
```

If you only want to capture some window:

```
ffmpeg.exe -f gdigrab -i title="Firefox Developer Edition" out.mp4
```

If you have 2 displays but want to capture only one:

```
ffmpeg.exe -f gdigrab -offset_x 0 -offset_y 0 -video_size 3840x2160 -i desktop out.mp4
```

#### DirectShow

First you need to install a capture device, for example [Screen Capture Recorder](https://github.com/rdp/screen-capture-recorder-to-video-windows-free).

```
ffmpeg.exe -f dshow -i audio="virtual-audio-capturer":video="screen-capture-recorder" -acodec pcm_s16le
-vcodec libx264 -preset ultrafast -qp 0 out.mp4
```

For more details read [my article](https://retifrav.github.io/blog/2017/04/24/record-the-screen-with-ffmpeg/) about screen recording with FFmpeg on Windows.

### Convert video to GIF

``` bash
ffmpeg.exe -i video.mov -pix_fmt rgb8 -r 15 -vf scale=700:-1 output.gif
```

* `-pix_fmt rgb8` - lowers the picture quality;
* `-r 15` - sets FPS to `15`;
* `-vf scale=700:-1` - sets the frame size.

### Convert video to Apple-compatible format

In order to convert your video file (even though it's already `mp4`) to an iMovie/QuickTime format (`yuv420p`):

``` bash
ffmpeg -i in.mp4 -pix_fmt yuv420p out.mp4
```

That also seems to be the most common value playing the video in web-browsers and more common players than MPV and VLC.

### Blur specific region for a period of time

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
* `boxblur=15` - the strength of blurring;
* `-map 0:a` - copies the audio stream. If you don't have audio in your video, then delete this.

### Convert FLAC to ALAC

To convert from FLAC (APE, MP3, whatever) to ALAC (Apple Lossless), do this:

``` bash
ffmpeg -i some.flac -c:a alac some.m4a
```

If you want to convert to some other format than ALAC, just set the right codec instead of `alac` (`aac`, for example).

If you need to convert several files, you can use one of the following scripts.

#### Mac OS / Linux

```bash
for f in ./*.flac; do ffmpeg -i "$f" -c:a alac "${f%.*}.m4a"; done
```

And if you want to delete originals, then:

```bash
for f in ./*.flac; do ffmpeg -i "$f" -c:a alac "${f%.*}.m4a" && rm "$f"; done
```

#### Windows

If your FFmpeg is in `C:\Program Files\ffmpeg\bin\ffmpeg.exe`, then:

``` pwsh
ls -recurse -include *.flac | %{& 'C:\Program Files\ffmpeg\bin\ffmpeg.exe' -i $_.FullName -map 0:0 -c:a alac ($_.BaseName+'.m4a')}
```

### Sync video and audio

If you have misaligned video and audio, for example you hear sounds before the acton really happens, then you need to offset the audio to the right on timeline.

For example, here's the original file with video and audio misaligned:

``` bash
|vvvvvvvvvvvvvvvvvv|
|aaaaaaaaaaaaaaaaaa|
```

Let's say, you need to offset the audio for 5 seconds. Here's the command:

``` bash
ffmpeg -i original.mp4 -itsoffset 5 -i original.mp4 -map 0:v -map 1:a -codec copy out.mp4
```

And that's how the synced file will look like:

``` bash
|vvvvvvvvvvvvvvvvvv|
|-----aaaaaaaaaaaaa|aaaaa
```

Note, that audio will get trimmed from end, so the last `aaaaa` gets deleted.

### Add audio to video

``` bash
ffmpeg -i video.mp4 -i audio.mp3 -codec copy -shortest output.mp4
```

* `-codec copy` - do not encode anything, just keep everything as it is;
* `-shortest` - truncate the longest input. Useful, if the audio is longer that video.

### Slow or speed up the video

``` bash
ffmpeg -i video.mp4 -crf 18 -filter:v "setpts=0.25*PTS" output.mp4
```

* `-filter:v "setpts=0.5*PTS"` - filter that sets a new speed of the video. `1` gives the same speed, `2.0` - 2x slower, `0.5` - 2x faster, `0.25` - 4x faster and so on.

### Rotate the video

``` bash
ffmpeg -i video.mov -vf "transpose=2" -crf 18 out.mp4
```

* `transpose` - how to rotate the video:
    - `0` - 90 counter-cLockwise and vertical flip (default);
    - `1` - 90 clockwise;
    - `2` - 90 counter-clockwise;
    - `3` - 90 clockwise and vertical flip.

If you want to rotate 90 counter-clockwise twice (so it's 180 degree of rotation in total), then just set it twice:

``` bash
ffmpeg -i video.mov -vf "transpose=2,transpose=2" -crf 18 out.mp4
```

### Apply several filters at once

Say, you want resize the video frame and also speed it up.

``` bash
ffmpeg -i some.mov -vf "scale=450:-1, setpts=0.5*PTS" -crf 18 out.mp4
```

or

``` bash
ffmpeg -i some.mov -vf "[in] scale=450:-1 [scl]; [scl] setpts=0.5*PTS [out]" -crf 18 out.mp4
```

### Make a video from images

```
ffmpeg -r 1/2 -i "concat:some-image.png|another-image.png" -pix_fmt yuv420p video.mp4
```

### ARF to MP4


First, download their [Webex player](https://www.webex.com/video-recording.html) and convert the recording to `.flv`. Then:

```
$ ffmpeg.exe -i screen.flv -i voip.flv -map 0:v:0 -map 1:a:0 -vsync vfr -vf mpdecimate -g 15 out.mp4
```

here:

- `-vf mpdecimate` - filter for skipping duplicate frames
- `-vsync vfr` - sync video and audio after skipping lots of frames
- `-g 15` - set new keyframes theoretically every 15 seconds
