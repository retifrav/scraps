## gallery-dl

CLI [tool](https://github.com/mikf/gallery-dl) for downloading media from various social networks and other places.

<!-- MarkdownTOC -->

- [Configuration](#configuration)
- [Avoiding getting banned](#avoiding-getting-banned)
- [Media sources](#media-sources)
    - [Instagram](#instagram)
        - [Credentials](#credentials)
        - [Download the available media](#download-the-available-media)
        - [Download only videos](#download-only-videos)
        - [Download media only since certain date](#download-media-only-since-certain-date)

<!-- /MarkdownTOC -->

### Configuration

<https://github.com/mikf/gallery-dl/blob/master/docs/gallery-dl-example.conf>

Create a `~/.config/gallery-dl/config.json` - this is the default location, otherwise you'll need to add `--config /path/to/your/config.json` to every command.

The base config with some limits for not getting banned too soon:

``` sh
{
    "downloader":
    {
        "rate": "2M"
        "sleep": [5.2, 11.2],
        "sleep-request": "8.5-19.1"
    },
    "extractor":
    {
        "instagram":
        {
            "...": "..."
        }
    }
}
```

### Avoiding getting banned

Services might ban/suspend your account for automatic downloads, so you might want to adjust the rate/sleep settings in the [config](#configuration). Another thing to try is to pipe the `pv`:

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ | pv --rate-limit 432K > ./downloads.log
```

Although I am not sure how exactly does that help, plus it shows a useless metric of the `downloads.log` file size instead of an actual progress (*which admittedly is difficult to establish, since the total size of the downloads is unknown*).

### Media sources

#### Instagram

##### Credentials

If you are using 2FA, log-in to your Instagram account in a web-browser, copy the `sessionid` cookie:

``` json
{
    "extractor":
    {
        "instagram":
        {
            "cookies":
            {
                "sessionid": "1584773341%3AIwkNQlYgfpx3nK%3A9%3AAYevM3RLZhW6JJm9_DO4o431aCuZt4iNlpdBdxfjGQ"
            }
        }
    }
}
```

Otherwise it might be enough to just provide credentials inline with `--username`/`--password`.

##### Download the available media

``` sh
$ gallery-dl https://www.instagram.com/helga_model/
```

If if fails at some point with something like this:

```
[instagram][error] HttpError: '400 Bad Request' for 'https://www.instagram.com/api/v1/feed/user/SOME-ID/'
```

Then go back to browser and refresh the page - it will show you a captcha. Having passed that, continue downloading.

##### Download only videos

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --filter "extension not in ('jpg', 'webp', 'gif')"
```

or:

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --filter "extension in ('mp4', 'webm', 'mov')"
```

or:

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --filter "extension == 'mp4'"
```

##### Download media only since certain date

``` sh
$ gallery-dl https://www.instagram.com/helga_model/ --filter "date > datetime(2022, 5, 23)" -v
```
