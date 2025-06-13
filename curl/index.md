## cURL

<!-- MarkdownTOC -->

- [HEAD request](#head-request)
- [Processing results](#processing-results)
    - [Status code](#status-code)
    - [Status code and response body](#status-code-and-response-body)
- [Download and upload](#download-and-upload)
    - [Download a file](#download-a-file)
    - [Download a folder](#download-a-folder)
    - [Upload a file](#upload-a-file)
- [POST request with a parametrized JSON body](#post-request-with-a-parametrized-json-body)
- [Get file size](#get-file-size)
- [Get location by IP](#get-location-by-ip)
- [Pipe URL from Python script to cURL](#pipe-url-from-python-script-to-curl)
- [Inspect outgoing request](#inspect-outgoing-request)

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

### Processing results

#### Status code

Check from a Bash script if JFrog Artifactory contains a certain file:

``` sh
#!/bin/bash

apiStatusCode=$(curl -I -H "X-JFrog-Art-Api:YOUR-API-KEY" "https://artifactory.YOUR.HOST/artifactory/etc/src/qt/5.15.2.tar.xz" -w "%{http_code}" -s -o ./request.log)
echo $apiStatusCode
if [[ -z "$apiStatusCode" || "$apiStatusCode" != '200' ]]; then
    cat ./request.txt
    echo "There seems to be no such file available in Artifactory"
    exit 1
else
    echo "Requested file is present in Artifactory"
fi
```

#### Status code and response body

``` sh
#!/bin/bash

curl -s -w "\n%{http_code}" 'https://swapi.dev/api/people/1?format=json' | {
    read httpResponseBody # must be read first
    read httpStatusCode # must be read second

    if [[ -z "$httpStatusCode" || "$httpStatusCode" != '200' ]]; then
        echo "[ERROR] HTTP status code: $httpStatusCode"
        exit 1
    fi

    someName=$(jq .name <<< "$httpResponseBody")
    echo "Someone's name: $someName"
}
```
```
Someone's name: "Luke Skywalker"
```

### Download and upload

#### Download a file

Such as fetch a file from JFrog Artifactory:

``` sh
$ curl -H 'X-JFrog-Art-Api:YOUR-API-KEY' -o qt-src.tar.xz 'https://artifactory.YOUR.HOST/artifactory/etc/src/qt/5.15.2.tar.xz'
```

#### Download a folder

That it cannot do, so you might need to use `wget`, however an even better option is `lftp`:

``` sh
$ lftp -c mirror https://some.host/data/folder/with/files/ ./files
```

#### Upload a file

Such as publish a file to JFrog Artifactory:

``` sh
$ curl -H "X-JFrog-Art-Api:YOUR-API-KEY" -H "X-Checksum-Sha1:b3760aadd696fa8009cf54eac0dd535c7886cc8f" -T ./qt-everywhere-src-5.15.2.tar.xz "https://artifactory.YOUR.HOST/artifactory/etc/src/qt/5.15.2.tar.xz"
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

### Get file size

``` sh
$ curl -sI https://hb.bizmrg.com/icq-www/mac/x64/icq.dmg | grep -i Content-Length | awk '{print $2/1024/1024 " MB"}'
186.251 MB
```

### Get location by IP

``` sh
$ curl ipinfo.io
```
```
{
  "ip": "THE-IP-YOU-REQUESTED-THIS-FROM",
  "city": "Amsterdam",
  "region": "North Holland",
  "country": "NL",
  "loc": "52.3740,4.8897",
  "org": "AS8075 Microsoft Corporation",
  "postal": "1012",
  "timezone": "Europe/Amsterdam",
  "readme": "https://ipinfo.io/missingauth"
}
```

or:

``` sh
$ curl -s ipinfo.io | jq .city
```
```
"Amsterdam"
```

### Pipe URL from Python script to cURL

There is some API, we need to get an URL from its JSON response and download a file from that URL:

``` sh
$ jsonContent=$(curl -s -H "Authorization: Bearer ACCESS-TOKEN" http://some.host/api/v1/some/content)
$ echo $jsonContent | python -c "import sys, json; sys.stdout.write('{0}{1}'.format('http://some.host/files/', json.load(sys.stdin)['content']['SDK']['Windows']['links']['MSVC 2019']))" | xargs curl
-s -H "Authorization: Bearer ACCESS-TOKEN" --write-out "%{http_code}" -O
```

### Inspect outgoing request

Such as to inspect what your JSON request body was turned into:

``` sh
$ export x='"one thing","another-thing"'
$ curl --trace-ascii ./curl.trace -X POST https://example.org -d "{\"one\": \"thing\",\"another\":\"stuff\",\"things\":[$x]}"
$ cat ./curl.trace
...
0000: POST / HTTP/2
000f: Host: example.org
0022: User-Agent: curl/8.7.1
003a: Accept: */*
0047: Content-Length: 73
005b: Content-Type: application/x-www-form-urlencoded
008c: 
=> Send data, 73 bytes (0x49)
0000: {"one": "thing","another":"stuff","things":["one thing","another
0040: -thing"]}
== Info: upload completely sent off: 73 bytes
...
```
