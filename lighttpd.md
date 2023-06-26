# lighttpd

<https://www.lighttpd.net>

<!-- MarkdownTOC -->

- [Basic usage](#basic-usage)
- [Custom HTTP headers](#custom-http-headers)
- [Enable directory browsing](#enable-directory-browsing)

<!-- /MarkdownTOC -->

## Basic usage

``` sh
$ nano ~/lighttpd.conf
```
``` cpp
server.document-root = "/var/www/some/" # "d:/temp/some/"

server.port = 8000

mimetype.assign = (
  ".html" => "text/html",
  ".css" => "text/css",
  ".js" => "text/javascript",
  ".wasm" : "application/wasm",
  ".jpg" => "image/jpeg",
  ".png" => "image/png",
  ".json" : "application/json",
  ".txt" => "text/plain"
)
```

Some plain-text file to serve:

``` sh
$ sudo mkdir /var/www/some/
$ sudo echo "ololo" > /var/www/some/file.txt
```

Launch:

``` sh
$ lighttpd -f ~/lighttpd.conf -D
```

here:

* `-f` - path to the config file;
* `-D` - do not run in background.

Open <http://localhost:8000/file.txt>.

## Custom HTTP headers

``` cpp
server.document-root = "/var/www/some/"

server.port = 8000

server.modules = (
  "mod_setenv"
)

setenv.add-response-header = (
  "Access-Control-Allow-Origin" => "*",
  "Access-Control-Allow-Headers" => "range"
)
```

## Enable directory browsing

``` cpp
server.document-root = "/var/www/some/"

server.port = 8000

server.dir-listing = "enable"
```
