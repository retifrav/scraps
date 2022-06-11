## gallery-dl

<https://github.com/mikf/gallery-dl>

<!-- MarkdownTOC -->

- [Instagram](#instagram)
    - [Credentials](#credentials)
    - [Download the available media](#download-the-available-media)
    - [Download only videos](#download-only-videos)
    - [Download media only since certain date](#download-media-only-since-certain-date)

<!-- /MarkdownTOC -->

### Instagram

#### Credentials

If you are using 2FA, log-in to your Instagram account in a web-browser, copy the `sessionid` cookie. Create a `config.json` file:

``` json
{
    "extractor": {
        "instagram": {
            "cookies": {
                "sessionid": "1034773341%3AkFGR902RNz6kGh%3V8"
            }
        },
    }
}
```

Otherwise it might be enough to just provide credentials inline with `--username`/`--password`.

#### Download the available media

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --config ./config.json
```

#### Download only videos

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --config ./config.json --filter "extension not in ('jpg', 'webp', 'gif')"
```

or:

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --config ./config.json --filter "extension in ('mp4', 'webm', 'mov')"
```

or:

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --config ./config.json --filter "extension == 'mp4'"
```

#### Download media only since certain date

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --config ./config.json --filter "date > datetime(2022, 5, 23)" -v
```
