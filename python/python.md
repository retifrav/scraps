## Python

<!-- MarkdownTOC -->

- [pip](#pip)
    - [Install specific version of a package](#install-specific-version-of-a-package)
    - [Re-install the package](#re-install-the-package)
- [Web server](#web-server)
    - [One-liner with default settings](#one-liner-with-default-settings)
    - [Custom script](#custom-script)
- [Dictionary as switch](#dictionary-as-switch)
- [Colors in print](#colors-in-print)
- [Sort dictionary by nested values](#sort-dictionary-by-nested-values)
- [Pretty print JSON](#pretty-print-json)
- [mypy](#mypy)

<!-- /MarkdownTOC -->

### pip

#### Install specific version of a package

``` sh
$ pip install -Iv dearpygui==1.1.3
```

#### Re-install the package

``` sh
$ pip install --upgrade --force-reinstall tap-adql-sandbox
```

### Web server

#### One-liner with default settings

Run in a folder to serve static HTML pages:

``` bash
python -m http.server 8000
```

#### Custom script

If you need to specify MIME types, then use this script instead:

``` python
import http.server
import socketserver

port = 8000


class HttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    extensions_map = {
        ".manifest": "text/cache-manifest",
        ".html": "text/html",
        ".png": "image/png",
        ".jpg": "image/jpg",
        ".svg":	"image/svg+xml",
        ".css":	"text/css",
        ".xml": "application/xml",
        ".json": "application/json",
        ".js": "application/javascript",
        ".wasm": "application/wasm",
        "": "application/octet-stream"
    }


httpd = socketserver.TCPServer(("localhost", port), HttpRequestHandler)

try:
    print(f"serving at http://localhost:{port}")
    httpd.serve_forever()
except KeyboardInterrupt:
    pass
```

### Dictionary as switch

``` python
statusCodesColors = {
    200: "green",
    301: "yellow",
    302: "yellow",
    401: "yellow",
    403: "yellow"
}


def getColorForStatus(statusCode):
    print(statusCodesColors.get(statusCode, "red"))


getColorForStatus(302)
```

### Colors in print

``` bash
pip install colorama
pip install termcolor
```

**Colorama** is needed for **termcolor** to work in Windows console (`cmd.exe`).

``` python
import colorama
from termcolor import colored

colorama.init()

print(colored("All good", "green"))
```

### Sort dictionary by nested values

You have the following dictionary:

``` json
{
  "dynamism": {
    "name": "Динамизм",
    "sum": 37.0,
    "avg": 7.4,
    "stdev": 0.548
  },
  "realism": {
    "name": "Реализм",
    "sum": 31.0,
    "avg": 6.2,
    "stdev": 2.168
  },
  "originality": {
    "name": "Оригинальность",
    "sum": 41.0,
    "avg": 8.2,
    "stdev": 1.643
  },
  "involvement": {
    "name": "Вовлечение",
    "sum": 37.0,
    "avg": 7.4,
    "stdev": 1.673
  }
}
```

To sort it by `sum` value descending:

``` python
>>> print(sorted(userMetrics, key=lambda x: (userMetrics[x]['sum']), reverse=True))

['originality', 'dynamism', 'involvement', 'realism']
```

### Pretty print JSON

If you want to print a dictionary as JSON:

``` python
import json

print(json.dumps(YOUR-DICTIONARY, indent=2))
```

### mypy

Like TypeScript but [for Python](http://mypy-lang.org/).

Check one file:

``` sh
$ mypy ./some.py
```

If it complains about missing types (`"pandas": module is installed, but missing library stubs`), either install those types for the packages it complains about:

``` sh
$ pip install pandas-stubs
```

or ignore them:

``` sh
$ mypy --ignore-missing-imports ./some.py
```

You can also check all the sources in the package:

``` sh
$ mypy --ignore-missing-imports .
```

or add that flag to `setup.cfg`:

``` ini
[mypy]
ignore_missing_imports = true
```
