## youtube-dl and yt-dlp

<!-- MarkdownTOC -->

- [Limit download speed](#limit-download-speed)
- [Format selection](#format-selection)
- [Specific format in config](#specific-format-in-config)
- [Subtitles](#subtitles)

<!-- /MarkdownTOC -->

### Limit download speed

To 1 MB/s:

``` sh
$ yt-dlp -r 1m https://here-goes-the.link
```

### Format selection

List available formats:

``` sh
$ yt-dlp -F https://youtu.be/9ahVgFZZOVg

ID  EXT   RESOLUTION FPS │   FILESIZE  TBR PROTO │ VCODEC       VBR ACODEC      ABR     ASR MORE INFO
──────────────────────────────────────────────────────────────────────────────────────────────────────────────
sb2 mhtml 48x27          │                 mhtml │ images                                   storyboard
sb1 mhtml 50x45          │                 mhtml │ images                                   storyboard
sb0 mhtml 101x90         │                 mhtml │ images                                   storyboard
139 m4a                  │    1.68MiB  48k https │ audio only       mp4a.40.5   48k 22050Hz low, m4a_dash
249 webm                 │    1.70MiB  49k https │ audio only       opus        49k 48000Hz low, webm_dash
250 webm                 │    2.24MiB  65k https │ audio only       opus        65k 48000Hz low, webm_dash
140 m4a                  │    4.46MiB 129k https │ audio only       mp4a.40.2  129k 44100Hz medium, m4a_dash
251 webm                 │    4.48MiB 130k https │ audio only       opus       130k 48000Hz medium, webm_dash
17  3gp   176x144      6 │    2.00MiB  58k https │ mp4v.20.3    58k mp4a.40.2    0k 22050Hz 144p
160 mp4   162x144     25 │    1.72MiB  49k https │ avc1.4d400b  49k video only              144p, mp4_dash
278 webm  162x144     25 │  770.96KiB  21k https │ vp9          21k video only              144p, webm_dash
133 mp4   270x240     25 │    3.75MiB 109k https │ avc1.4d400d 109k video only              240p, mp4_dash
242 webm  270x240     25 │    1.45MiB  42k https │ vp9          42k video only              240p, webm_dash
134 mp4   406x360     25 │    2.78MiB  80k https │ avc1.4d4015  80k video only              360p, mp4_dash
18  mp4   406x360     25 │   12.00MiB 348k https │ avc1.42001E 348k mp4a.40.2    0k 44100Hz 360p
243 webm  406x360     25 │    2.69MiB  78k https │ vp9          78k video only              360p, webm_dash
135 mp4   540x480     25 │    5.32MiB 154k https │ avc1.4d401e 154k video only              480p, mp4_dash
244 webm  540x480     25 │    5.51MiB 160k https │ vp9         160k video only              480p, webm_dash
136 mp4   810x720     25 │   10.59MiB 307k https │ avc1.4d401f 307k video only              720p, mp4_dash
22  mp4   810x720     25 │ ~ 15.41MiB 436k https │ avc1.64001F 436k mp4a.40.2    0k 44100Hz 720p
247 webm  810x720     25 │   14.82MiB 430k https │ vp9         430k video only              720p, webm_dash
```

Download highest quality audio only (*ID `140`*):

``` sh
$ yt-dlp -f 140 https://youtu.be/9ahVgFZZOVg
```

### Specific format in config

`~/.config/youtube-dl/config`:

```
--format="bestvideo[height<=?1080]+bestaudio/best[height<=?1080]"
```

`~/.config/yt-dlp/config`:

```
-f "bv[height=1080][ext=mp4]+ba / bv*+ba/b"
```

### Subtitles

List available subtitles:

``` sh
$ yt-dlp https://tv.nrk.no/serie/gutta-paa-skauen/sesong/1/episode/1 --list-subs
[NRKTVEpisode] Extracting URL: https://tv.nrk.no/serie/gutta-paa-skauen/sesong/1/episode/1
[NRKTVEpisode] gutta-paa-skauen/sesong/1/episode/1: Downloading webpage
[NRK] Extracting URL: nrk:MUHH45000122
[NRK] MUHH45000122: Downloading manifest JSON
[NRK] MUHH45000122: Downloading m3u8 information
[NRK] MUHH45000122: Downloading metadata JSON
[NRK] MUHH45000122: Downloading programs JSON
[info] Available subtitles for MUHH45000122:
Language Formats
nb-nor   vtt
nb-ttv   vtt
```

Download selected language and convert to SRT:

``` sh
$ yt-dlp https://tv.nrk.no/serie/gutta-paa-skauen/sesong/1/episode/1 --skip-download --write-sub --sub-lang nb-nor --convert-subs=srt
[NRKTVEpisode] Extracting URL: https://tv.nrk.no/serie/gutta-paa-skauen/sesong/1/episode/1
[NRKTVEpisode] gutta-paa-skauen/sesong/1/episode/1: Downloading webpage
[NRK] Extracting URL: nrk:MUHH45000122
[NRK] MUHH45000122: Downloading manifest JSON
[NRK] MUHH45000122: Downloading m3u8 information
[NRK] MUHH45000122: Downloading metadata JSON
[NRK] MUHH45000122: Downloading programs JSON
[info] MUHH45000122: Downloading subtitles: nb-nor
[info] MUHH45000122: Downloading 1 format(s): 4486
[info] Writing video subtitles to: Gutta på skauen - 1. Den femte mann [MUHH45000122].nb-nor.vtt
[download] Destination: Gutta på skauen - 1. Den femte mann [MUHH45000122].nb-nor.vtt
[download] 100% of    2.92KiB in 00:00:00 at 18.34KiB/s
[SubtitlesConvertor] Converting subtitles
Deleting original file Gutta på skauen - 1. Den femte mann [MUHH45000122].nb-nor.vtt (pass -k to keep)
```

If downloaded subtitles are too short, try downloading all the available languages with `--sub-lang all`.
