## youtube-dl and yt-dlp

<!-- MarkdownTOC -->

- [Limit download speed](#limit-download-speed)
- [Format selection](#format-selection)

<!-- /MarkdownTOC -->

### Limit download speed

To 1 MB/s:

``` sh
$ yt-dlp -r 1m https://here-goes-the.link
```

### Format selection

`~/.config/youtube-dl/config`:

```
--format="bestvideo[height<=?1080]+bestaudio/best[height<=?1080]"
```

`~/.config/yt-dlp/config`:

```
-f "bv[height=1080][ext=mp4]+ba / bv*+ba/b"
```
