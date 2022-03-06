## cURL

<!-- MarkdownTOC -->

- [Send a HEAD request](#send-a-head-request)
- [Send a request and get response status code](#send-a-request-and-get-response-status-code)
- [Upload a file](#upload-a-file)
- [Download a file](#download-a-file)

<!-- /MarkdownTOC -->

### Send a HEAD request

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

### Send a request and get response status code

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
