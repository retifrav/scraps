## FFmpeg

Also [official documentation](https://ffmpeg.org//ffmpeg.html) and [collection of filters with live preview](https://ffmpeg.lav.io).

<!-- MarkdownTOC -->

- [Cutting](#cutting)
    - [Cut video fragment](#cut-video-fragment)
    - [Cut out part of the video](#cut-out-part-of-the-video)
- [Subtitles](#subtitles)
    - [Keep subtitles](#keep-subtitles)
    - [Extract subtitles](#extract-subtitles)
    - [Mux dvd_subtitle from one container to another](#mux-dvd_subtitle-from-one-container-to-another)
        - [Adjusting dvd_subtitle position and stretch](#adjusting-dvd_subtitle-position-and-stretch)
    - [Burn subtitles into the video](#burn-subtitles-into-the-video)
- [Tracks](#tracks)
    - [Remove unwanted tracks](#remove-unwanted-tracks)
    - [Choose between audio tracks](#choose-between-audio-tracks)
    - [Add an audio track to container](#add-an-audio-track-to-container)
    - [Sync video and audio](#sync-video-and-audio)
- [Video encoding](#video-encoding)
- [Resize video](#resize-video)
    - [Downgrade certain VR videos](#downgrade-certain-vr-videos)
        - [Wrong pixel format](#wrong-pixel-format)
- [Concat several files](#concat-several-files)
    - [Concat a lot of files](#concat-a-lot-of-files)
- [Watermark on each frame](#watermark-on-each-frame)
- [Crop video](#crop-video)
- [Screen capture](#screen-capture)
    - [GDI](#gdi)
    - [DirectShow](#directshow)
- [Convert video to GIF](#convert-video-to-gif)
    - [Fast and dirty](#fast-and-dirty)
    - [Properly](#properly)
- [Convert video to Apple-compatible format](#convert-video-to-apple-compatible-format)
- [Blur specific region for a period of time](#blur-specific-region-for-a-period-of-time)
- [Fill the video with solid color](#fill-the-video-with-solid-color)
- [Convert FLAC to ALAC](#convert-flac-to-alac)
    - [Mac OS / Linux](#mac-os--linux)
    - [Windows](#windows)
- [Convert WAV 24 bit 5.1 to ALAC 16 bit stereo](#convert-wav-24-bit-51-to-alac-16-bit-stereo)
- [Slow or speed up the video](#slow-or-speed-up-the-video)
- [Rotate the video](#rotate-the-video)
- [Apply several filters at once](#apply-several-filters-at-once)
- [Make a video from images](#make-a-video-from-images)
    - [When you don't have a lot of images](#when-you-dont-have-a-lot-of-images)
    - [When you have a lot of images](#when-you-have-a-lot-of-images)
- [ARF to MP4](#arf-to-mp4)
- [Merge audiobook files into one](#merge-audiobook-files-into-one)
- [Fix aspect ratio](#fix-aspect-ratio)
- [How many channels does an audio track have](#how-many-channels-does-an-audio-track-have)
- [Remux DVD](#remux-dvd)
    - [Getting rid of black bars](#getting-rid-of-black-bars)

<!-- /MarkdownTOC -->

### Cutting

#### Cut video fragment

``` sh
$ ffmpeg -ss 00:03:05 -i 1.mp4 -t 00:01:06 -c copy cut.mp4
```

This will cut 66 seconds (00:01:06) from `1.mp4` starting from 00:03:05 timestamp and save it to `cup.mp4`. Putting `-ss 00:03:05` before `-i` [makes it](https://stackoverflow.com/a/33188399/) to start cutting from the nearest keyframe, so you won't have frozen frames or shit in your output video.

If you still get bad results such as messed up keyframes and weird timings, especially on short cuts, try to drop `-codec copy`, so it's just:

``` sh
$ ffmpeg -ss 00:00:06 -i out.mp4 -t 00:00:11 cut.mp4
```

If that helps, then find the right re-encoding parameters to get quality closest to the original (*audio can stay the same with `-c:a copy`*).

Instead of `-t` (*the length to cut starting from the `-ss` time*) there can be `-to` (*till which time it should be cut from the `-ss` time*). Both `-t` and `-to` can be either `00:01:40` or `100` (*1 minute and 40 seconds*).

Also, if the original video has chapters, you'll probably want to drop them - add `-map_chapters -1`.

#### Cut out part of the video

<https://stackoverflow.com/a/50599108/1688203>

We want to cut out the fragment from 222-nd second to 231-st second, given that total video length is 255 seconds:

``` sh
$ ffmpeg -i ./some.mov \
    -vf "select='between(t,0,222)+between(t,231,255)',setpts=N/FRAME_RATE/TB" \
    -profile:v high \
    ./out.mp4
```

If the video has audio track too, then:

``` sh
$ ffmpeg -i ./some.mov \
    -vf "select='between(t,0,222)+between(t,231,255)',setpts=N/FRAME_RATE/TB" \
    -af "aselect='between(t,0,222)+between(t,231,255)',asetpts=N/SR/TB" \
    -profile:v high \
    ./out.mp4
```

### Subtitles

#### Keep subtitles

By default(?) FFmpeg maps only the first video track and the first audio track, so if you also have subtitles in the video, then the following command will "drop" the subtitles:

``` sh
$ ffmpeg -i ./some.mp4 -crf 18 ./some-crf.mp4
```

To keep the subtitles you could try to map all the tracks:

``` sh
Stream #0:0[0x1](und): Video: h264 (High) (avc1 / 0x31637661), yuv420p(tv, bt709/bt709/unknown, progressive), 1920x1080, 3682 kb/s, SAR 1:1 DAR 16:9, 16 fps, 16 tbr, 16384 tbn (default)
  Metadata:
    creation_time   : 2026-05-27T13:17:42.000000Z
    handler_name    : VideoHandler
    encoder         : H.264
    timecode        : 00:00:00:00
Stream #0:1[0x2](und): Audio: aac (LC) (mp4a / 0x6134706D), 48000 Hz, stereo, fltp, 319 kb/s (default)
  Metadata:
    creation_time   : 2026-05-27T13:17:42.000000Z
    handler_name    : SoundHandler
Stream #0:2[0x3](eng): Subtitle: mov_text (tx3g / 0x67337874), 0 kb/s (default)
  Metadata:
    creation_time   : 2026-05-27T13:17:42.000000Z
    handler_name    : SubtitleHandler
Stream #0:3[0x4](eng): Data: none (tmcd / 0x64636D74) (default)
  Metadata:
    creation_time   : 2026-05-27T13:17:42.000000Z
    handler_name    : TimeCodeHandler
    timecode        : 00:00:00:00
# ...

$ ffmpeg -i ./some.mp4 -map 0 -crf 18 ./some-crf.mp4
```

but it will likely fail with:

``` sh
[mp4 @ 0x7eec27c00] Using non-standard frame rate 16/1
[mp4 @ 0x7eec27c00] You requested a copy of the original timecode track so timecode metadata are now ignored
[mp4 @ 0x7eec27c00] Could not find tag for codec none in stream #3, codec not currently supported in container
[out#0/mp4 @ 0x7eec50480] Could not write header (incorrect codec parameters ?): Invalid argument
[vf#0:0 @ 0x7ee800000] Error sending frames to consumers: Invalid argument
[vf#0:0 @ 0x7ee800000] Task finished with error code: -22 (Invalid argument)
[vf#0:0 @ 0x7ee800000] Terminating thread with return code -22 (Invalid argument)
[out#0/mp4 @ 0x7eec50480] Nothing was written into output file, because at least one of its streams received no packets.
```

so you'll have to map them explicitly and specify the `mov_text` codec for subtitles:

``` sh
$ ffmpeg -i ./some.mp4 -map 0:0 -crf 18 -map 0:1 -map 0:2 -c:s mov_text ./some-crf.mp4
```

#### Extract subtitles

Check the file's info and discover the subtitles track number. After that:

``` sh
$ ffmpeg -i 1.mkv -map 0:2 subtitles.ass
```

If subtitles aren't text but `dvd_subtitle`, then:

``` sh
$ ffmpeg -i ./1.mkv -map 0:2 -c copy -f matroska subtitles.mks
```

#### Mux dvd_subtitle from one container to another

``` sh
$ ffprobe -i ./1.mkv
# ...
Stream #0:0(eng): Video: h264 (High), yuv420p(progressive), 1920x816 [SAR 1:1 DAR 40:17], 25 fps, 25 tbr, 1k tbn (default)

$ ffprobe -i ./2.mkv
# ...
Stream #0:0: Video: mpeg2video (Main), yuv420p(tv, top first), 720x576 [SAR 16:15 DAR 4:3], 25 fps, 25 tbr, 1k tbn
# ...
Stream #0:3(rus): Subtitle: dvd_subtitle (dvdsub)
    Metadata:
      VIEWPORT        : Fullscreen
      DURATION        : 01:32:48.600000000
```

Trying to mux them directly:

``` sh
$ ffmpeg -i ./1.mkv -i ./2.mkv -map 0 -map 1:3 -c copy ./3.mkv
```

will result in subtitles positioned in the left-middle part (*where the original frame size had them*), so instead you need to extract them first:

``` sh
$ mkvextract ./2.mkv tracks 3:subtitles
Extracting track 3 with the CodecID 'S_VOBSUB' to the file 'subtitles.sub'. Container format: VobSubs
Writing the VobSub index file 'subtitles.idx'.
Progress: 100%
```

Edit the `subtitles.idx` to add `size: 720x576` line:

``` sh
# VobSub index file, v7 (do not modify this line!)
palette: 0000fd, ce2110, 000000, fdfdfd, 00fe00, fc00fd, fefe00, 007c7b, 7c7c7c, e0e0e0, 7a0000, 007d00, 00007a, db00fd, dc7d00, 7c007b
size: 720x576
langidx: 0

id: ru, index: 0
timestamp: 00:00:15:360, filepos: 000000000
timestamp: 00:02:19:520, filepos: 000001000
# ...
```

and then merge:

``` sh
$ mkvmerge -o ./3.mkv ./1.mkv --language 0:rus subtitles.idx
mkvmerge v103.0 ('The Darkness') 64-bit
'./1.mkv': Using the demultiplexer for the format 'Matroska'.
'subtitles.idx': Using the demultiplexer for the format 'VobSub'.
'./1.mkv' track 0: Using the output module for the format 'AVC/H.264'.
'./1.mkv' track 1: Using the output module for the format 'AC-3'.
'./1.mkv' track 2: Using the output module for the format 'AC-3'.
'subtitles.idx' track 0: Using the output module for the format 'VobSub'.
The file './3.mkv' has been opened for writing.
The cue entries (the index) are being written...
Multiplexing took 3 seconds.
```

##### Adjusting dvd_subtitle position and stretch

Resulting subtitles might get stretched horizontally and positioned a bit too high, and that can be adjusted with this [Python script](https://github.com/retifrav/python-scripts/blob/ea62480ca217964fce3d566d0d4fbff6b23ffc68/vobsub-adjust/vobsub-adjust.py).

First check the headroom:

``` sh
$ python ./vobsub-adjust.py ./subtitles.idx --info --video-height 816
Canvas 720x576, 850 subtitles
  Text now sits in rows 475-529
  Room below the text: median 46 px (about 65 video px), smallest 46 px (about 65 video px)
  Transparent space under the text inside its display area: median 45 px (about 64 video px)
  Text width: median 437 px, widest 616 px
```

The `816` height is from the video where you got them muxed in. And from the report it's `65 video px` what matters, so that's how much lower you can reposition the subtitles untill that stop lowering and will simply stay at the lowest point at the frame bottom border.

Then the actual adjustment to reposition subtitles 50 pixels lower and also make them less wide:

``` sh
$ python ./vobsub-adjust.py subtitles.idx subtitles-adjusted.idx --shift-y 50 --scale-x 0.9 --video-height 816
Canvas 720x576, 850 subtitles
  Text now sits in rows 475-529
  Room below the text: median 46 px (about 65 video px), smallest 46 px (about 65 video px)
  Transparent space under the text inside its display area: median 45 px (about 64 video px)
  Text width: median 437 px, widest 616 px
  After: text in rows 510-564
Wrote subtitles-adjusted.idx and subtitles-adjusted.sub
```

Finally, merge them into the container:

``` sh
$ mkvmerge -o ./3.mkv ./1.mkv --language 0:rus subtitles-adjusted.idx
mkvmerge v103.0 ('The Darkness') 64-bit
'./1.mkv': Using the demultiplexer for the format 'Matroska'.
'subtitles-adjusted.idx': Using the demultiplexer for the format 'VobSub'.
'./1.mkv' track 0: Using the output module for the format 'AVC/H.264'.
'./1.mkv' track 1: Using the output module for the format 'AC-3'.
'./1.mkv' track 2: Using the output module for the format 'AC-3'.
'subtitles-adjusted.idx' track 0: Using the output module for the format 'VobSub'.
The file './3.mkv' has been opened for writing.
The cue entries (the index) are being written...
Multiplexing took 3 seconds.
```

So for comparison, here's how the subtitles looked like before adjustment:

![](./img/der-grosse-bagarozy-dvd-subtitle-original.jpg?raw=true "Der Grosse Bagarozy frame with muxed original dvd_subtitle")

and this is how they look now adjusted:

![](./img/der-grosse-bagarozy-dvd-subtitle-adjusted.jpg?raw=true "Der Grosse Bagarozy frame with muxed adjusted dvd_subtitle")

#### Burn subtitles into the video

``` sh
$ ffmpeg -i ./galactic-emperor.mp4 \
    -vf "subtitles=./galactic-emperor.srt:force_style='FontName=Verdana Bold'" \
    ./out.mp4
```

If you want non-bold Verdana, then use `force_style='FontName=Verdana'` instead. If you don't want to use Verdana font at all, then keep just the `-vf subtitles=./galactic-emperor.srt`.

You can have other styling too, for example red color:

``` sh
$ ffmpeg -i ./galactic-emperor.mp4 \
    -vf "subtitles=./galactic-emperor.srt:force_style='FontName=Verdana Bold,PrimaryColour=&H0000ff&'" \
    ./out.mp4
```

### Tracks

#### Remove unwanted tracks

Original file:

``` sh
$ ffprobe -i ~/Downloads/Up.In.The.Air.2009.WEB-DL.2160p.Rutracker.mkv
Input #0, matroska,webm, from '/Users/vasya/Downloads/Up.In.The.Air.2009.WEB-DL.2160p.Rutracker.mkv':
  Metadata:
    encoder         : libebml v1.4.4 + libmatroska v1.7.1
    creation_time   : 2024-01-22T10:53:41.000000Z
  Duration: 01:49:20.06, start: 0.000000, bitrate: 28188 kb/s
  Stream #0:0: Video: hevc (Main 10), yuv420p10le(tv, bt2020nc/bt2020/smpte2084), 3840x2076 [SAR 1:1 DAR 320:173], 23.98 fps, 23.98 tbr, 1k tbn (default) (original)
      Metadata:
        BPS             : 24600478
        DURATION        : 01:49:19.970000000
        NUMBER_OF_FRAMES: 157282
        NUMBER_OF_BYTES : 20172299736
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
      Side data:
        DOVI configuration record: version: 1.0, profile: 8, level: 6, rpu flag: 1, el flag: 0, bl flag: 1, compatibility id: 1
  Stream #0:1(rus): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s (default)
      Metadata:
        title           : Dub, Bluray
        BPS             : 640000
        DURATION        : 01:49:19.328000000
        NUMBER_OF_FRAMES: 204979
        NUMBER_OF_BYTES : 524746240
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:2(rus): Audio: ac3, 48000 Hz, 5.1(side), fltp, 448 kb/s
      Metadata:
        title           : MVO
        BPS             : 448000
        DURATION        : 01:49:20.064000000
        NUMBER_OF_FRAMES: 205002
        NUMBER_OF_BYTES : 367363584
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:3(rus): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s
      Metadata:
        title           : AVO, Сербин
        BPS             : 640000
        DURATION        : 01:49:19.328000000
        NUMBER_OF_FRAMES: 204979
        NUMBER_OF_BYTES : 524746240
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:4(rus): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s
      Metadata:
        title           : VO, Яковлев
        BPS             : 640000
        DURATION        : 01:49:19.328000000
        NUMBER_OF_FRAMES: 204979
        NUMBER_OF_BYTES : 524746240
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:5(ukr): Audio: ac3, 48000 Hz, 5.1(side), fltp, 384 kb/s
      Metadata:
        title           : DUB, Студія "LeDoyen"
        BPS             : 384000
        DURATION        : 01:49:18.784000000
        NUMBER_OF_FRAMES: 204962
        NUMBER_OF_BYTES : 314821632
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:6(ukr): Audio: ac3, 48000 Hz, stereo, fltp, 192 kb/s
      Metadata:
        title           : MVO, 1+1
        BPS             : 192000
        DURATION        : 01:49:18.784000000
        NUMBER_OF_FRAMES: 204962
        NUMBER_OF_BYTES : 157410816
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:7(eng): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s
      Metadata:
        title           : Original
        BPS             : 640000
        DURATION        : 01:49:18.752000000
        NUMBER_OF_FRAMES: 204961
        NUMBER_OF_BYTES : 524700160
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:8(rus): Subtitle: subrip (srt) (dub)
      Metadata:
        title           : Full, WEB-DL
        BPS             : 110
        DURATION        : 01:43:24.621000000
        NUMBER_OF_FRAMES: 1447
        NUMBER_OF_BYTES : 85723
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:9(rus): Subtitle: subrip (srt)
      Metadata:
        title           : Full, Шнайдер
        BPS             : 124
        DURATION        : 01:46:10.931000000
        NUMBER_OF_FRAMES: 1204
        NUMBER_OF_BYTES : 99321
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:10(ukr): Subtitle: subrip (srt) (dub)
      Metadata:
        title           : Full
        BPS             : 115
        DURATION        : 01:43:24.412000000
        NUMBER_OF_FRAMES: 1455
        NUMBER_OF_BYTES : 89765
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:11(eng): Subtitle: subrip (srt) (original)
      Metadata:
        title           : Full
        BPS             : 78
        DURATION        : 01:48:24.562000000
        NUMBER_OF_FRAMES: 1805
        NUMBER_OF_BYTES : 63575
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
  Stream #0:12(eng): Subtitle: subrip (srt) (original)
      Metadata:
        title           : SDH
        BPS             : 86
        DURATION        : 01:48:40.138000000
        NUMBER_OF_FRAMES: 2007
        NUMBER_OF_BYTES : 70351
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
```

Keep original english audio track (*and make it default*), keep one russian track (*and remove the `default` attribute from it*) and two subtitles:

``` sh
$ ffmpeg -i ~/Downloads/Up.In.The.Air.2009.WEB-DL.2160p.Rutracker.mkv \
    -map 0:v:0 -map 0:a:6 -map 0:a:0 -map 0:s:3 -map 0:s:0 \
    -disposition:a:1 -default -disposition:a:0 default \
    -c copy \
    ./Up.In.The.Air.2009.WEB-DL.2160p.Rutracker.mkv
```

Result:

``` sh
$ ffprobe -i ./Up.In.The.Air.2009.WEB-DL.2160p.Rutracker.mkv
Input #0, matroska,webm, from './Up.In.The.Air.2009.WEB-DL.2160p.Rutracker.mkv':
  Metadata:
    ENCODER         : Lavf61.1.100
  Duration: 01:49:19.97, start: 0.000000, bitrate: 25885 kb/s
  Stream #0:0: Video: hevc (Main 10), yuv420p10le(tv, bt2020nc/bt2020/smpte2084), 3840x2076 [SAR 1:1 DAR 320:173], 23.98 fps, 23.98 tbr, 1k tbn (default) (original)
      Metadata:
        BPS             : 24600478
        NUMBER_OF_FRAMES: 157282
        NUMBER_OF_BYTES : 20172299736
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
        DURATION        : 01:49:19.969000000
      Side data:
        DOVI configuration record: version: 1.0, profile: 8, level: 6, rpu flag: 1, el flag: 0, bl flag: 1, compatibility id: 1
  Stream #0:1(eng): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s (default)
      Metadata:
        title           : Original
        BPS             : 640000
        NUMBER_OF_FRAMES: 204961
        NUMBER_OF_BYTES : 524700160
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
        DURATION        : 01:49:18.752000000
  Stream #0:2(rus): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s
      Metadata:
        title           : Dub, Bluray
        BPS             : 640000
        NUMBER_OF_FRAMES: 204979
        NUMBER_OF_BYTES : 524746240
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
        DURATION        : 01:49:19.328000000
  Stream #0:3(eng): Subtitle: subrip (srt) (original)
      Metadata:
        title           : Full
        BPS             : 78
        NUMBER_OF_FRAMES: 1805
        NUMBER_OF_BYTES : 63575
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
        DURATION        : 01:49:05.107000000
  Stream #0:4(rus): Subtitle: subrip (srt) (dub)
      Metadata:
        title           : Full, WEB-DL
        BPS             : 110
        NUMBER_OF_FRAMES: 1447
        NUMBER_OF_BYTES : 85723
        _STATISTICS_WRITING_APP: mkvmerge v77.0 ('Elemental') 64-bit
        _STATISTICS_WRITING_DATE_UTC: 2024-01-22 10:53:41
        _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
        DURATION        : 01:46:26.553000000
```

#### Choose between audio tracks

Following [that](#cut-out-part-of-the-video) cutting example:

``` sh
$ ffmpeg -i 1.mp4 -hide_banner
...
Duration: 01:52:52.80, start: 0.000000, bitrate: 2768 kb/s
  Stream #0:0: Video: mpeg4 (XVID / 0x44495658), yuv420p, 704x384 [SAR 1:1 DAR 11:6]
  Stream #0:1: Audio: ac3 ([0] [0][0] / 0x2000), 48000 Hz, 5.1(side), s16, 448 kb/s
  Stream #0:2: Audio: ac3 ([0] [0][0] / 0x2000), 48000 Hz, 5.1(side), s16, 448 kb/s
...
```

So, we want 63 seconds of video and second audio track:

``` sh
$ ffmpeg -i 1.mkv -map 0:0 -map 0:2 -ss 01:37:34 -t 63 -vcodec copy -acodec copy cut.mkv
```

#### Add an audio track to container

If it doesn't have its own audio:

``` sh
$ ffmpeg -i video.mp4 -i audio.mp3 -codec copy -shortest output.mp4
```

* `-codec copy` - do not encode anything, just keep everything as it is;
* `-shortest` - truncate the longest input. Useful, if the audio is longer that video.

If it does already have its own audio track:

``` sh
$ ffmpeg -i Tropic.Thunder.UNRATED.1080p.BluRay.x264-HD1080.mkv -i ru.ac3 \
-map 0:v -map 0:a:0 -map 1:a \
-metadata:s:a:0 language=eng -metadata:s:a:1 language=rus \
-codec copy \
-shortest \
Tropic.Thunder.UNRATED.1080p.BluRay.x264-HD1080-ENG-RUS.mkv
```

or, if you don't want to map every single track from the original audio, plus if you'd like to add some metadata:

``` sh
$ ffmpeg -i /path/to/Deja-Vu-2006-720p-BluRay-DD51-x264-DON.mkv \
-i /path/to/audio-rus-dub.ac3 -map 0 -map 1:a \
-metadata:s:a:2 title="Russian (dubbing)" -metadata:s:a:2 language=rus \
-c copy ./out.mkv
```

Here the original video already has 2 audio tracks (*the main one and commentary*), and so the new track that we are adding will be the 3rd (*starting with `0` for the main, `1` for commentary and so this one is `2`*) and we are setting its language and title.

#### Sync video and audio

If you have misaligned video and audio, for example you hear sounds before the action really happens, then you need to offset the audio to the right on timeline.

For example, here's the original file with video and audio misaligned:

``` sh
|vvvvvvvvvvvvvvvvvv|
|aaaaaaaaaaaaaaaaaa|
```

Let's say, you need to offset the audio for 5 seconds. Here's the command:

``` sh
$ ffmpeg -i original.mp4 -itsoffset 5 -i original.mp4 -map 0:v -map 1:a -codec copy out.mp4
```

And that's how the synced file will look like:

``` sh
|vvvvvvvvvvvvvvvvvv|
|-----aaaaaaaaaaaaa|aaaaa
```

Note, that audio will get trimmed from end, so the last `aaaaa` gets deleted.

### Video encoding

``` sh
$ ffmpeg -i 1.avi -crf 18 out.mp4
```

* `-crf` - some kind of "level of quality" from `0` (best) to `51` (worst). Value `18` is "[visually lossless or nearly so](https://trac.ffmpeg.org/wiki/Encode/H.264#a1.ChooseaCRFvalue)".

### Resize video

To make it smaller (frame dimensions), mostly.

``` sh
$ ffmpeg -i some.mov -vf scale=1022:-1 output.mp4
```

* `-vf scale=1022:-1` - output video will have `1022` width, and height value will be calculated correspondingly. It is possible to pick *wrong* values, and then FFmpeg will tell you something like `Error while opening encoder for output stream #0:0 - maybe incorrect parameters such as bit_rate, rate, width or height` or `height not divisible by 2` - adjust `scale` value a bit.

This can be avoided by automatically picking the right dimension. For example:

``` sh
$ ffmpeg -i original.mp4 -vf scale="trunc(oh*a/2)*2:720" out.mp4
```

or even just:

``` sh
$ ffmpeg -i original.mp4 -vf scale="-2:720" out.mp4
```

#### Downgrade certain VR videos

So, your computer isn't that powerful to handle certain 5k/6k/7k/etc videos with H.265/HEVC, but you really want to "watch" them:

``` sh
$ ffmpeg -i ./Mina-Von-D-Playing-With-Fire.mp4 -vf scale=-1:2048 -c:v h264_nvenc -b:v 25M -profile:v high -c:a copy ./Mina-Von-D-Playing-With-Fire-x264-4k-25m.mp4
```

or, if your system is capable of playing H.265/HEVC, then perhaps you can perform downscaling with that codec instead of H.264:

``` sh
$ ffmpeg -i ./Mina-Von-D-Playing-With-Fire.mp4 -vf scale=-1:2048 -c:v hevc_nvenc -b:v 25M -c:a copy ./Mina-Von-D-Playing-With-Fire-x265-4k-25m.mp4
```

In my case encoding/downscaling went faster, although the resulting file size was somewhat bigger, but most importantly it was still freezing on playing, so not on my calculator.

##### Wrong pixel format

If conversion/downscaling fails with:

```
[h264_nvenc @ 000002c707e81580] 10 bit encode not supported
[h264_nvenc @ 000002c707e81580] No capable devices found
Error initializing output stream 0:0 -- Error while opening encoder for output stream #0:0 - maybe incorrect parameters such as bit_rate, rate, width or height
```

then add `-pix_fmt yuv420p`.

### Concat several files

Concat 2 videos:

```
$ ffmpeg -i first.mp4 -i second.mp4 -filter_complex "[0:v] [0:a] [1:v] [1:a] concat=n=2:v=1:a=1 [v] [a]" -map "[v]" -map "[a]" out.mp4
```

Or maybe 3 videos:

```
$ ffmpeg -i first.mp4 -i second.mp4 -i third.mp4 -filter_complex "[0:v] [0:a] [1:v] [1:a] [2:v] [2:a] concat=n=3:v=1:a=1 [v] [a]" -map "[v]" -map "[a]" out.mp4
```

#### Concat a lot of files

``` sh
$ ls -1v *.mp4 > files.txt
```

On Mac OS you might need to use `gls` instead of `ls`.

Edit the file:

```
file '1.mp4'
file '2.mp4'
file '3.mp4'
file '4.mp4'
file '5.mp4'
```

If your files have the same codec, then concat them without re-encoding:

``` sh
$ ffmpeg -f concat -safe 0 -i files.txt -c copy output.mp4
```

Otherwise, concat with encoding:

``` sh
$ ffmpeg -f concat -safe 0 -i files.txt output.mp4
```

If you need to download a YouTube playlist, which has videos in different formats, then download them like this:

``` sh
$ youtube-dl -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]/mp4' --recode-video mp4 https://www.youtube.com/playlist?list=PLAYLIST-ID -o '%(autonumber)s-%(title)s'
```

Audio files can be joined the same way, which is especially useful for audio-books. In that case you might even drop the encoding:

``` sh
$ ffmpeg -f concat -safe 0 -i files.txt -codec copy output.mp3
```

### Watermark on each frame

``` sh
$ ffmpeg -i 1.mp4 -vf "movie=logo.png [logo]; [in][logo] overlay=16:16[out]" -crf 18 2.mkv
```

Watermark file `logo.png` is in the same directory. Value `16:16` sets coordinates for top-left corner of watermark image.

### Crop video

Say, you have a video with `1366x768` resolution and you want to crop 300 pixels from the left:

``` sh
$ ffmpeg -i 1.mp4 -filter:v "crop=1066:768:300:0" -crf 18 cut.mp4
```

First pair (`1066:768`) sets a new frame size, and second pair (`300:0`) sets coordinates (`x:y`) for its top-left (*left-top, actually*) corner relatively from the original.

To make the guess-work easier, capture a frame from the video and open it in some image editor. There you can use a crop tool to find the exact values in pixels of the area of interest and its offsets.

### Screen capture

The `mp4` is not good for recording, because should anything happen the whole file will end up being corrupted, while `mkv` will (*hopefully*) handle this gracefully. After recording is done, "repack" the `*.mkv` file to `*.mp4` (*without re-encoding*) for [browsers compatibility](#convert-video-to-apple-compatible-format).

#### GDI

```
$ ffmpeg -f gdigrab -i desktop out.mkv
```

If you only want to capture some window:

```
$ ffmpeg -f gdigrab -i title="Firefox Developer Edition" out.mkv
```

If you want to capture a region:

```
$ ffmpeg -f gdigrab -offset_x 18 -offset_y 149 -video_size 1296x536 -show_region 1 -i desktop out.mkv
```

You can get the region of interest using [Screen Coordinates Tool](http://breakthrusoftware.com/html/onlinedocs/kb/installkb/ScreenCoordTool.html).

If you have 2 displays but want to capture only the first left one (*with resolution `3840x2160`*):

```
$ ffmpeg -f gdigrab -offset_x 0 -offset_y 0 -video_size 3840x2160 -show_region 1 -i desktop out.mkv
```

You can also try to improve encoding performance with NVIDIA hardware acceleration:

```
$ ffmpeg -f gdigrab -i desktop -c:v h264_nvenc out.mkv
```

#### DirectShow

First you need to install a capture device, for example [Screen Capture Recorder](https://github.com/rdp/screen-capture-recorder-to-video-windows-free).

```
ffmpeg -f dshow -i audio="virtual-audio-capturer":video="screen-capture-recorder" -acodec pcm_s16le
-vcodec libx264 -preset ultrafast -qp 0 out.mkv
```

For more details read [my article](https://retifrav.github.io/blog/2017/04/24/record-the-screen-with-ffmpeg/) about screen recording with FFmpeg on Windows.

### Convert video to GIF

#### Fast and dirty

``` sh
$ ffmpeg -i video.mov -r 10 -vf scale=640:-1 output.gif
```

* `-r 10` - sets FPS to `10`
* `-vf scale=640:-1` - sets the frame size width to `640` (*if you would like to downscale*)

Result:

![](./img/ffmpeg-gif-dirty.gif?raw=true "FFmpeg, fast and dirty GIF")

#### Properly

In two steps:

``` sh
$ ffmpeg -i some.mov -vf fps=10,scale=640:-1:flags=lanczos,palettegen palette.png
$ ffmpeg -i some.mov -i palette.png -filter_complex "fps=10,scale=640:-1:flags=lanczos[x];[x][1:v]paletteuse" output.gif
```

In one step:

```
$ ffmpeg -i some.mov -vf "fps=10,scale=640:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" output.gif
```

Result:

![](./img/ffmpeg-gif-proper.gif?raw=true "FFmpeg, proper GIF")

### Convert video to Apple-compatible format

In order to convert your video file (even though it's already `mp4`) to an iMovie/QuickTime format (`yuv420p`):

``` sh
$ ffmpeg -i in.mp4 -pix_fmt yuv420p out.mp4
```

That also seems to be the most common value playing the video in web-browsers and more common players than MPV and VLC.

### Blur specific region for a period of time

We have a `1280x720` video and we want to blur some region like this (*in school I would get some punishment for placing sizes that way*):

![](./img/ffmpeg-blurred-region.png?raw=true "FFmpeg, blur specifig region")

We need to apply a complex filter:

``` sh
$ ffmpeg -i some.mp4 -filter_complex \
 "[0:v]crop=490:500:790:220,boxblur=15[fg]; \
  [0:v][fg]overlay=790:220:enable='between(t,1,10)'[v]" \
-map "[v]" -map 0:a -crf 18 -movflags +faststart blurred.mp4
```

* `490:500` - width and height of the region we want to blur;
* `790:220` - **x** and **y** coordinates of the top-left corner for this region;
* `enable='between(t,1,10)'` - enables blurring only from 00:00:01 to 00:00:10 timestamps (9 seconds in total) of the video;
* `boxblur=15` - the strength of blurring;
* `-map 0:a` - copies the audio stream. If you don't have audio in your video, then delete this.

### Fill the video with solid color

Fill the entire frame with black color from 00:01:30 till 00:02:00 time:

``` sh
$ ffmpeg -i video.mp4 -vf "drawbox=x=0:y=0:w=in_w:h=in_h:color=black@1.0:t=fill:enable='between(t,90,120)'" out.mp4
```

Fill part of the frame with solid pink color from 00:00:00.125 till 00:00:02.033 time:

``` sh
$ ffmpeg -i ./video.mp4 -vf "drawbox=x=360:y=240:w=1200:h=530:color=pink@1.0:t=fill:enable='between(t,0.125,2.033)" ./out.mp4
```

Fill part of the frame with green color strip of 50px height on 50% opacity for the entire video length:

``` sh
$ ffmpeg -i video.mp4 -vf "drawbox=x=0:y=0:w=in_w:h=50:color=green@0.5:t=fill" out.mp4
```

### Convert FLAC to ALAC

To convert from FLAC (APE, MP3, whatever) to ALAC (Apple Lossless), do this:

``` sh
$ ffmpeg -i some.flac -c:a alac some.m4a
```

If you want to convert to some other format than ALAC, just set the right codec instead of `alac` (`aac`, for example).

If you need to convert several files, you can use one of the following scripts.

#### Mac OS / Linux

``` sh
$ for f in ./*.flac; do ffmpeg -i "$f" -c:a alac "${f%.*}.m4a"; done
```

And if you want to delete originals, then:

``` sh
$ for f in ./*.flac; do ffmpeg -i "$f" -c:a alac "${f%.*}.m4a" && rm "$f"; done
```

#### Windows

If your FFmpeg is in `C:\Program Files\ffmpeg\bin\ffmpeg.exe`, then:

``` pwsh
$ ls -recurse -include *.flac | %{& 'C:\Program Files\ffmpeg\bin\ffmpeg.exe' -i $_.FullName -map 0:0 -c:a alac ($_.BaseName+'.m4a')}
```

### Convert WAV 24 bit 5.1 to ALAC 16 bit stereo

If you got a crazy 24 bit 5.1 WAV album, you can convert it to a "normal" stereo 16 bit ALAC:

``` sh
$ for f in ./*.wav; do ffmpeg -i "$f" -af aformat=s16:44100 -ac 2 -c:a alac "${f%.*}.m4a"; done
```

### Slow or speed up the video

``` sh
$ ffmpeg -i ./video.mp4 -crf 18 -filter:v "setpts=0.25*PTS" ./out.mp4
```

* `-filter:v "setpts=0.5*PTS"` - filter that sets a new speed of the video. `1` gives the same speed, `2.0` - 2x slower, `0.5` - 2x faster, `0.25` - 4x faster and so on.

If there is an audio too, its speed also needs to be changed, for example here everything will be made 2.5 times faster:

``` sh
ffmpeg -i ./video.mp4 -vf "setpts=(PTS-STARTPTS)/2.5" -af "atempo=2.5" ./out.mp4
```

### Rotate the video

``` sh
$ ffmpeg -i video.mov -vf "transpose=2" -crf 18 out.mp4
```

* `transpose` - how to rotate the video:
    - `0` - 90 counter-cLockwise and vertical flip (default);
    - `1` - 90 clockwise;
    - `2` - 90 counter-clockwise;
    - `3` - 90 clockwise and vertical flip.

If you want to rotate 90 counter-clockwise twice (so it's 180 degree of rotation in total), then just set it twice:

``` sh
$ ffmpeg -i video.mov -vf "transpose=2,transpose=2" -crf 18 out.mp4
```

### Apply several filters at once

Say, you want resize the video frame and also speed it up.

``` sh
$ ffmpeg -i some.mov -vf "scale=450:-1, setpts=0.5*PTS" -crf 18 out.mp4
```

or

``` sh
$ ffmpeg -i some.mov -vf "[in] scale=450:-1 [scl]; [scl] setpts=0.5*PTS [out]" -crf 18 out.mp4
```

### Make a video from images

#### When you don't have a lot of images

``` sh
$ ffmpeg -r 1/2 -i "concat:some-image.png|another-image.png" -pix_fmt yuv420p video.mp4
```

Here's a [more sophisticated variant](https://superuser.com/a/834035/682702) and with fade effect too:

``` sh
$ ffmpeg \
-loop 1 -t 2 -i the-queens-gambit.png \
-loop 1 -t 2 -i devs.png \
-loop 1 -t 2 -i interrogation.png \
-loop 1 -t 2 -i run.png \
-loop 1 -t 2 -i the-outsider.png \
-loop 1 -t 2 -i messiah.png \
-loop 1 -t 2 -i ted-lasso.png \
-loop 1 -t 2 -i perry-mason.png \
-loop 1 -t 2 -i breeders.png \
-loop 1 -t 2 -i the-eddy.png \
-loop 1 -t 2 -i outer-banks.png \
-filter_complex \
"[0:v]fade=t=out:st=2:d=1[v0]; \
 [1:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v1]; \
 [2:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v2]; \
 [3:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v3]; \
 [4:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v4]; \
 [5:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v5]; \
 [6:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v6]; \
 [7:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v7]; \
 [8:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v8]; \
 [9:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v9]; \
 [10:v]fade=t=in:st=0:d=1,fade=t=out:st=2:d=1[v10]; \
 [v0][v1][v2][v3][v4][v5][v6][v7][v8][v9][v10]concat=n=11:v=1:a=0,format=yuv420p[v]" -map "[v]" out.mp4
```

#### When you have a lot of images

They need to have filenames of the same length:

``` sh
$ ls -L1 ./frms/ | head -5
000.png
001.png
002.png
003.png
004.png
```

And then:

``` sh
$ ffmpeg -i ./frms/%03d.png -c:v libx264 -pix_fmt yuv420p -r 30 ./out.mp4
```

If `-r` doesn't work for you for some reason, try `fps` instead:

``` sh
$ ffmpeg -i ./frms/%03d.png -c:v libx264 -pix_fmt yuv420p -vf fps=30 ./out.mp4
```

Some more examples [here](https://trac.ffmpeg.org/wiki/Slideshow).

### ARF to MP4

First, download their [Webex player](https://www.webex.com/video-recording.html) and convert the recording to `.flv`. Then:

```
$ ffmpeg -i screen.flv -i voip.flv -map 0:v:0 -map 1:a:0 -vsync vfr -vf mpdecimate -g 15 out.mp4
```

here:

- `-vf mpdecimate` - filter for skipping duplicate frames
- `-vsync vfr` - sync video and audio after skipping lots of frames
- `-g 15` - set new keyframes theoretically every 15 seconds

### Merge audiobook files into one

Often audiobooks come as a set of many files. Who knows what purpose does this splitting serve, but if you a sane person, then most likely you would like to have just one file per audiobook. To do so you'll need to make an alphabetically(?) sorted list of its files and concat them into one like this (*recipe for Mac OS and MP3*):

``` sh
$ gls -1v *.mp3 | while IFS= read -r i; do echo "file '$i'"; done > files.txt
$ less ./files.txt # to check that it got sorted correctly
$ ffmpeg -f concat -safe 0 -i ./files.txt -codec copy ./output.mp3
```

Looping through `ls` results is in general a [bad idea](http://mywiki.wooledge.org/ParsingLs), so make sure that your files are named without weird symbols or anything like that.

If you know that files names might contain single quotes, then you can escape them with `sed`:

``` sh
$ gls -1v *.mp3 | sed "s/'/'\\\\''/g; s/.*/file '&'/" > files.txt
```

which also gets rid of the `while ... do` loop, because `sed` already does its operations in a loop itself.

### Fix aspect ratio

Some video might have craze values in SAR/DAR metadata, so playing those will result in crazy frame size, even though the actual frame resolution is okay. To fix those videos you don't need to re-encode, just fix the metadata. Find out the actual frame resolution (*with the same `ffmpeg`/`ffprobe`*) and use those values to set `-aspect`:

``` sh
$ ffmpeg -i ./video-wth-crazy-metadata-ratio.mp4 -aspect 1280:800 -c:v copy -c:a copy ./video-fixed.mp4
```

### How many channels does an audio track have

``` sh
$ ffprobe ./Deadpool.2016.UHD.BluRay.2160p.TrueHD.Atmos.7.1.HEVC.REMUX-FraMeSToR.mkv
# ...
  Stream #0:1(eng): Audio: truehd (Dolby TrueHD + Dolby Atmos), 48000 Hz, 7.1, s32 (24 bit) (default)
    Metadata:
      title           : TrueHD Atmos 7.1
      BPS             : 5024481
      BPS-eng         : 5024481
      DURATION-eng    : 01:48:06.230000000
      NUMBER_OF_FRAMES: 7783476
      NUMBER_OF_FRAMES-eng: 7783476
      NUMBER_OF_BYTES : 4073742712
      NUMBER_OF_BYTES-eng: 4073742712
      _STATISTICS_WRITING_APP: mkvmerge v18.0.0 ('Apricity') 64-bit
      _STATISTICS_WRITING_APP-eng: mkvmerge v18.0.0 ('Apricity') 64-bit
      _STATISTICS_WRITING_DATE_UTC: 2017-11-27 23:42:19
      _STATISTICS_WRITING_DATE_UTC-eng: 2017-11-27 23:42:19
      _STATISTICS_TAGS: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
      _STATISTICS_TAGS-eng: BPS DURATION NUMBER_OF_FRAMES NUMBER_OF_BYTES
      DURATION        : 01:48:06.230000000
# ...

$ ffprobe -v error \
  -select_streams a:0 \
  -show_entries stream=codec_name,channels,sample_rate \
  -of default=noprint_wrappers=1 \
  ./Deadpool.2016.UHD.BluRay.2160p.TrueHD.Atmos.7.1.HEVC.REMUX-FraMeSToR.mkv
codec_name=truehd
sample_rate=48000
channels=8

$ ffprobe -v error \
  -select_streams a:0 \
  -show_entries stream=channels \
  -of default=noprint_wrappers=1:nokey=1 \
  ./Deadpool.2016.UHD.BluRay.2160p.TrueHD.Atmos.7.1.HEVC.REMUX-FraMeSToR.mkv
8
```

### Remux DVD

To remux DVD files (*`VIDEO_TS`*) such as this:

``` sh
$ mediainfo VTS_01_0.IFO 
General
Complete name                            : VTS_01_0.IFO
Format                                   : DVD Video
File size                                : 62.0 KiB
Duration                                 : 1 h 37 min
Overall bit rate mode                    : Variable
Overall bit rate                         : 9 170 kb/s
Frame rate                               : 25.000 FPS
Writing library                          : TMPGEnc (ver. 2.524.63.181)

Video
ID                                       : 224 (0xE0)
Format                                   : MPEG Video
Format version                           : Version 2
Format profile                           : Main@Main
Format settings                          : CustomMatrix / BVOP
Format settings, BVOP                    : Yes
Format settings, Matrix                  : Custom
Format settings, GOP                     : M=3, N=15
Duration                                 : 1 h 37 min
Bit rate mode                            : Variable
Bit rate                                 : 8 092 kb/s
Width                                    : 720 pixels
Height                                   : 576 pixels
Display aspect ratio                     : 4:3
Frame rate                               : 25.000 FPS
Standard                                 : PAL
Color space                              : YUV
Chroma subsampling                       : 4:2:0
Bit depth                                : 8 bits
Compression mode                         : Lossy
Bits/(Pixel*Frame)                       : 0.780
Time code of first frame                 : 00:00:00:00
Time code source                         : Group of pictures header
GOP, Open/Closed                         : Closed
Stream size                              : 904 MiB
Writing library                          : TMPGEnc 2.524.63.181
Color primaries                          : BT.601 PAL
Transfer characteristics                 : BT.470 System B/G
Matrix coefficients                      : BT.470 System B/G
intra_dc_precision                       : 10

Audio #1
ID                                       : 189 (0xBD)-128 (0x80)
Format                                   : AC-3
Format/Info                              : Audio Coding 3
Commercial name                          : Dolby Digital
Muxing mode                              : DVD-Video
Duration                                 : 1 h 37 min
Bit rate mode                            : Constant
Bit rate                                 : 448 kb/s
Channel(s)                               : 6 channels
Channel layout                           : L R C LFE Ls Rs
Sampling rate                            : 48.0 kHz
Frame rate                               : 31.250 FPS (1536 SPF)
Compression mode                         : Lossy
Delay relative to video                  : 6 ms
Stream size                              : 50.0 MiB
Language                                 : Russian
Service kind                             : Complete Main
bsid                                     : 8
Dialog Normalization                     : -27
Dialog Normalization                     : -27 dB
compr                                    : -8.16
compr                                    : -8.16 dB
dynrng                                   : -6.44
dynrng                                   : -6.44 dB
acmod                                    : 7
lfeon                                    : 1
cmixlev                                  : -3.0
cmixlev                                  : -3.0 dB
surmixlev                                : -3 dB
surmixlev                                : -3 dB
mixlevel                                 : 105
mixlevel                                 : 105 dB
roomtyp                                  : Small
dialnorm_Average                         : -27
dialnorm_Average                         : -27 dB
dialnorm_Minimum                         : -27
dialnorm_Minimum                         : -27 dB
dialnorm_Maximum                         : -27
dialnorm_Maximum                         : -27 dB
dialnorm_Count                           : 270
compr_Average                            : -6.70
compr_Average                            : -6.70 dB
compr_Minimum                            : -8.89
compr_Minimum                            : -8.89 dB
compr_Maximum                            : -3.66
compr_Maximum                            : -3.66 dB
compr_Count                              : 270
dynrng_Average                           : -3.89
dynrng_Average                           : -3.89 dB
dynrng_Minimum                           : -7.18
dynrng_Minimum                           : -7.18 dB
dynrng_Maximum                           : -1.48
dynrng_Maximum                           : -1.48 dB
dynrng_Count                             : 270

Audio #2
ID                                       : 189 (0xBD)-129 (0x81)
Format                                   : AC-3
Format/Info                              : Audio Coding 3
Commercial name                          : Dolby Digital
Muxing mode                              : DVD-Video
Duration                                 : 1 h 37 min
Bit rate mode                            : Constant
Bit rate                                 : 448 kb/s
Channel(s)                               : 6 channels
Channel layout                           : L R C LFE Ls Rs
Sampling rate                            : 48.0 kHz
Frame rate                               : 31.250 FPS (1536 SPF)
Compression mode                         : Lossy
Delay relative to video                  : 6 ms
Stream size                              : 50.0 MiB
Language                                 : German
Service kind                             : Complete Main
bsid                                     : 8
Dialog Normalization                     : -27
Dialog Normalization                     : -27 dB
compr                                    : -8.16
compr                                    : -8.16 dB
dynrng                                   : -6.44
dynrng                                   : -6.44 dB
acmod                                    : 7
lfeon                                    : 1
cmixlev                                  : -3.0
cmixlev                                  : -3.0 dB
surmixlev                                : -3 dB
surmixlev                                : -3 dB
mixlevel                                 : 105
mixlevel                                 : 105 dB
roomtyp                                  : Small
dialnorm_Average                         : -27
dialnorm_Average                         : -27 dB
dialnorm_Minimum                         : -27
dialnorm_Minimum                         : -27 dB
dialnorm_Maximum                         : -27
dialnorm_Maximum                         : -27 dB
dialnorm_Count                           : 270
compr_Average                            : -6.71
compr_Average                            : -6.71 dB
compr_Minimum                            : -9.27
compr_Minimum                            : -9.27 dB
compr_Maximum                            : -4.08
compr_Maximum                            : -4.08 dB
compr_Count                              : 270
dynrng_Average                           : -4.32
dynrng_Average                           : -4.32 dB
dynrng_Minimum                           : -7.18
dynrng_Minimum                           : -7.18 dB
dynrng_Maximum                           : -1.64
dynrng_Maximum                           : -1.64 dB
dynrng_Count                             : 270

Text
ID                                       : 189 (0xBD)-32 (0x20)
Format                                   : RLE
Format/Info                              : Run-length encoding
Duration                                 : 1 h 37 min
Bit depth                                : 2 bits

Menu
Duration                                 : 1 h 37 min
00:00:00.000                             : Chapter 1
00:00:13.160                             : Chapter 2
00:03:18.560                             : Chapter 3
00:07:07.160                             : Chapter 4
00:13:39.560                             : Chapter 5
00:19:03.560                             : Chapter 6
00:23:32.960                             : Chapter 7
00:31:28.160                             : Chapter 8
00:35:07.160                             : Chapter 9
00:40:08.960                             : Chapter 10
00:44:49.760                             : Chapter 11
00:50:59.360                             : Chapter 12
00:57:45.560                             : Chapter 13
01:00:41.960                             : Chapter 14
01:05:43.160                             : Chapter 15
01:11:46.160                             : Chapter 16
01:20:44.960                             : Chapter 17
01:24:17.960                             : Chapter 18
01:28:20.360                             : Chapter 19
01:34:59.360                             : Chapter 20
01:37:23.360                             : Chapter 21
List (Audio)                             : 0 / 1
List (Subtitles 4/3)                     : 0
  Wide)                                  : 0
  Letterbox)                             : 0
  Pan&Scan)                              : 0
```

...into a single MKV file, first check which title is the actual movie:

``` sh
$ for t in $(seq 1 10); do
    printf '%s: ' "$t"
    ffprobe -v error -f dvdvideo -title "$t" -i /path/to/PARENT-OF-VIDEO_TS/VIDEO_TS \
        -show_entries format=duration -of csv=p=0 || echo
done
```

It will print a wall of text with lots of errors, but none of those matter, you only need to find the longest title:

``` sh
1: libdvdread: Encrypted DVD support unavailable.
libdvdread: Setting up builtin libdvdread implementation
[dvdvideo @ 0x5558ae4e3c80] libdvdread: Couldn't find device name.
libdvdread: Encrypted DVD support unavailable.
libdvdread: Setting up builtin libdvdread implementation
[dvdvideo @ 0x5558ae4e3c80] libdvdnav: Couldn't find device name.
[dvdvideo @ 0x5558ae4e3c80] libdvdnav: Can't read name block. Probably not a DVD-ROM device.
[dvdvideo @ 0x5558ae4e3c80] libdvdnav: findDirFile: Could not open dir ./VIDEO_TS/VIDEO_TS/ 
[dvdvideo @ 0x5558ae4e3c80] libdvdnav: findDirFile: Could not open dir ./VIDEO_TS/video_ts/ 
5846.120000
```

which luckly is the 1st one here, because its length is `5846.12` seconds, so it's about 1 hour and 37 minutes, which definitely looks like a movie length; but you might not always get so lucky, so you might need to increase the `$(seq 1 10)` iteration length from 10 to something bigger.

Having found the longest title, you can remux it into MKV like this:

``` sh
$ ffmpeg -f dvdvideo -preindex 1 -title 1 -i ./path/to/PARENT-OF-VIDEO_TS -map 0 -c copy ./movie.mkv
```

The `-preindex 1` here is not the number of the title but a boolean value. So if your movie was at title 5, then you'd still had `-preindex 1` but `-title 5`.

One last thing to do is tag the tracks with languages. Here I know that subtitles are in russian and I also want to make the original german audio to be default:

``` sh
$ mkvpropedit ./movie.mkv \
    --edit track:s1 --set language=rus \
    --edit track:a1 --set flag-default=0 \
    --edit track:a2 --set flag-default=1
The file is being analyzed.
The changes are written to the file.
Done.
```

#### Getting rid of black bars

Of course an MKV remuxed from DVD will end up with black bars around the actual video frame. With a different DVD as an example:

``` sh
$ mediainfo VTS_05_0.IFO 
General
Complete name                            : VTS_05_0.IFO
Format                                   : DVD Video
File size                                : 62.0 KiB
Duration                                 : 1 h 19 min
Overall bit rate mode                    : Variable
Overall bit rate                         : 5 987 kb/s
Frame rate                               : 25.000 FPS

Video
ID                                       : 224 (0xE0)
Format                                   : MPEG Video
Format version                           : Version 2
Format profile                           : Main@Main
Format settings                          : CustomMatrix / BVOP
Format settings, BVOP                    : Yes
Format settings, Matrix                  : Custom
Format settings, GOP                     : M=3, N=12
Format settings, picture structure       : Frame
Duration                                 : 1 h 19 min
Bit rate mode                            : Variable
Bit rate                                 : 5 420 kb/s
Maximum bit rate                         : 7 000 kb/s
Width                                    : 720 pixels
Height                                   : 576 pixels
Display aspect ratio                     : 4:3
Frame rate                               : 25.000 FPS
Standard                                 : PAL
Color space                              : YUV
Chroma subsampling                       : 4:2:0
Bit depth                                : 8 bits
Scan type                                : Interlaced
Scan order                               : Top Field First
Compression mode                         : Lossy
Bits/(Pixel*Frame)                       : 0.523
Time code of first frame                 : 01:00:01:20
Time code source                         : Group of pictures header
GOP, Open/Closed                         : Closed
Stream size                              : 927 MiB
intra_dc_precision                       : 10
```

to measure the bars in the resulting remux:

``` sh
$ ffmpeg -ss 00:10:00 -i ./movie.mkv -t 120 -vf cropdetect=limit=24:round=2 -f null - 2>&1 \
    | grep -o 'crop=[0-9:]*' \
    | sort \
    | uniq -c \
    | sort -n \
    | tail -1
```

That will return something like `2873 crop=710:450:0:62`, which means:

- width: `710`;
- height: `450`;
- lines off the top: `62`;
- lines off the bottom: `576 - 450 - 62 = 64`

The reported width `710` being less than original `720` from the DVD means that there probably needs to be a horizontal crop too, likely from the right, which apparently is common on DVD masters. Here you can see it on the right:

![](./img/daun-haus-10px-bar-right.png?raw=true "Даун Хаус frame with a black bar on the right")

To get rid of the black bars usually you would have to re-encode the video, but you can try to fight this with so-called "container crop" by simply adjusting the MKV metadata (*if you'll decide to crop those horizontal 10 pixels too, then also add `--set pixel-crop-right=10`, but players might not like it*):

```sh
$ mkvpropedit ./movie.mkv \
    --edit track:v1 \
    --set pixel-crop-top=62 \
    --set pixel-crop-bottom=64
```

And then proper players will be able to display the video nicely cropped.

If you decide to do the re-encoding instead, then check for interlacing first:

``` sh
$ ffmpeg -i movie.mkv -t 60 -vf idet -f null -
# ...
[Parsed_idet_0 @ 0x7fbea4003c80] Repeated Fields: Neither:  1501 Top:     0 Bottom:     0
[Parsed_idet_0 @ 0x7fbea4003c80] Single frame detection: TFF:    40 BFF:    32 Progressive:  1072 Undetermined:   357
[Parsed_idet_0 @ 0x7fbea4003c80] Multi frame detection: TFF:    52 BFF:    25 Progressive:  1420 Undetermined:     4
[out#0/null @ 0x5645367c8380] video:621KiB audio:33462KiB subtitle:0KiB other streams:0KiB global headers:0KiB muxing overhead: unknown
frame= 1500 fps=0.0 q=-0.0 Lsize=N/A time=00:01:00.01 bitrate=N/A speed= 205x elapsed=0:00:00.29
```

This result says that the movie is progressive, so it doesn't need deinterlacing: 1420 progressive frames out of total 1501 (*so about 95%*), 52 TFF, 25 BFF and 4 undetermined (*if it was interlaced, then TFF would be the majority, not progressive*). So then encoding:

``` sh
$ ffmpeg -i movie.mkv -map 0 -c copy \
    -vf crop=710:450:0:62,setfield=prog \
    -c:v libx264 -preset slow -crf 16 -tune film \
    movie.dvdrip.mkv
```
