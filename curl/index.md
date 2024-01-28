## cURL

<!-- MarkdownTOC -->

- [HEAD request](#head-request)
- [Get file size](#get-file-size)
- [Get response status code](#get-response-status-code)
- [Upload a file](#upload-a-file)
- [Download a file](#download-a-file)
- [POST request with a parametrized JSON body](#post-request-with-a-parametrized-json-body)

<!-- /MarkdownTOC -->

### HEAD request

``` sh
$ curl -I https://YOUR.HOST

HTTP/1.1 200 OK
Server: nginx/1.18.0 (Ubuntu)
Date: Fri, 25 Feb 2022 13:56:11 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 22467
Last-Modified: Mon, 07 Feb 2022 09:48:06 GMT
Connection: keep-alive
ETag: "6200ead6-57c3"
Accept-Ranges: bytes
```

Get web-server:

``` sh
$ curl -s -I duckduckgo.com | awk '$1~/Server:/ {print $2}'
```

### Get file size

``` sh
$ curl -sI https://hb.bizmrg.com/icq-www/mac/x64/icq.dmg | grep -i Content-Length | awk '{print $2/1024/1024 " MB"}'
186.251 MB
```

### Get response status code

Check from a Bash script if JFrog Artifactory contains a certain file:

``` sh
#!/bin/bash

apiStatusCode=$(curl -I -H "X-JFrog-Art-Api:YOUR-API-KEY" "https://artifactory.YOUR.HOST/artifactory/etc/src/qt/5.15.2.tar.xz" -w "%{http_code}" -s -o ./request.log)
echo $apiStatusCode
if [[ -z "$apiStatusCode" || "$apiStatusCode" != 200 ]]; then
    cat ./request.txt
    echo "There seems to be no such file available in Artifactory"
    exit 1
else
    echo "Requested file is present in Artifactory"
fi
```

### Upload a file

Such as publish a file to JFrog Artifactory:

``` sh
$ curl -H "X-JFrog-Art-Api:YOUR-API-KEY" -H "X-Checksum-Sha1:b3760aadd696fa8009cf54eac0dd535c7886cc8f" -T ./qt-everywhere-src-5.15.2.tar.xz "https://artifactory.YOUR.HOST/artifactory/etc/src/qt/5.15.2.tar.xz"
```

### Download a file

Such as fetch a file from JFrog Artifactory:

``` sh
$ curl -H 'X-JFrog-Art-Api:YOUR-API-KEY' -o qt-src.tar.xz 'https://artifactory.YOUR.HOST/artifactory/etc/src/qt/5.15.2.tar.xz'
```

### POST request with a parametrized JSON body

For example, to send message via Telegram bot:

``` sh
$ export SLURM_JOB_ID=12321
$ jq -nc --arg msg "🌌 job #<code>$SLURM_JOB_ID</code> is done" '{"chat_id": "YOUR-TELEGRAM-ID", "text": $msg, "parse_mode": "HTML", "disable_web_page_preview": "true"}' \
    | curl -s -X POST -H "Content-Type: application/json" -d @- \
    https://api.telegram.org/botYOUR-TELEGRAM-BOT-API-TOKEN/sendMessage \
    > /dev/null
```
