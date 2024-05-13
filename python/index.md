## Python

<!-- MarkdownTOC -->

- [Install Python without installer and with pip](#install-python-without-installer-and-with-pip)
    - [Windows](#windows)
    - [GNU/Linux](#gnulinux)
- [pip](#pip)
    - [Install specific version of a package](#install-specific-version-of-a-package)
    - [Install a package with retarded versioning](#install-a-package-with-retarded-versioning)
    - [Update a package](#update-a-package)
    - [Re-install the package](#re-install-the-package)
    - [Uninstall all packages](#uninstall-all-packages)
- [Web server](#web-server)
    - [One-liner with default settings](#one-liner-with-default-settings)
    - [Custom script](#custom-script)
- [Dictionary as switch](#dictionary-as-switch)
- [Colors in print](#colors-in-print)
- [Sort dictionary by nested values](#sort-dictionary-by-nested-values)
- [Pretty print JSON](#pretty-print-json)
- [mypy](#mypy)
    - [More strict checks](#more-strict-checks)
- [SQL database](#sql-database)
    - [Connecting to PostgreSQL database](#connecting-to-postgresql-database)
    - [Simple SELECT](#simple-select)
    - [Parametrized SELECT](#parametrized-select)
- [Generate a random string](#generate-a-random-string)
- [SHA checksum of a file](#sha-checksum-of-a-file)
- [Virtual environment](#virtual-environment)
    - [Using custom virtual environment in Visual Studio Code](#using-custom-virtual-environment-in-visual-studio-code)
- [Set HTTP proxy](#set-http-proxy)
- [Closest value from a list](#closest-value-from-a-list)
- [Create a nested dictionary from a list of nodes](#create-a-nested-dictionary-from-a-list-of-nodes)
- [Convert Jupyter Notebook to HTML](#convert-jupyter-notebook-to-html)

<!-- /MarkdownTOC -->

### Install Python without installer and with pip

#### Windows

1. Download the archive, such as <https://www.python.org/ftp/python/3.10.8/python-3.10.8-embed-amd64.zip>;
2. Unpack it to `/d/programs/python/`, add that path to `PATH`;
    + in a new console check `python --version`;
3. Edit `python310._pth`, uncomment `import site`;
4. They say Python has a standard module for installing pip with `python -m ensurepip --upgrade`, but that's a goddamn lie, at least in case of embeddable Python package;
5. So download pip from <https://bootstrap.pypa.io/get-pip.py> and run `python ./get-pip.py`. Admittedly, that feels quite dangerous - executing scripts with blobs from the internet;
6. Add `/d/programs/python/Scripts/` to `PATH`;
    + in a new console check `pip --version`;
    + and install the module for virtual environment: `pip install virtualenv`;
        * and duplicate it as `venv`: `cp -r /d/programs/python/Lib/site-packages/{virtualenv,venv}`

#### GNU/Linux

When APT/deb repositories only have some old versions, like 3.8 on Ubuntu 20.04, if you don't want to add 3rd-party PPAs, you can build newer Python from sources:

``` sh
$ sudo apt install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev libsqlite3-dev wget libbz2-dev

$ wget https://www.python.org/ftp/python/3.11.3/Python-3.11.3.tgz
$ tar -xvf Python-3.11.3.tgz
$ cd ./Python-3.11.3/
$ ./configure --enable-optimizations
$ time make -j $(nproc)
$ sudo make altinstall
```

The `altinstall` is used instead of `install` in order to keep the default Python binary path in `/usr/bin/python`.

```
$ ls -L1 /usr/local/bin/python3.1*
/usr/local/bin/python3.11
/usr/local/bin/python3.11-config

$ ls -L1 /usr/local/bin/pip3.1*
/usr/local/bin/pip3.11
```

### pip

#### Install specific version of a package

``` sh
$ pip install -Iv dearpygui==1.1.3
```

#### Install a package with retarded versioning

Some packages might use a retarted (*non-[SemVer](https://semver.org)*) versioning, and then trying to install those will fail like this:

``` sh
Discarding https://files.pythonhosted.org/packages/78/fd/0148f0e54f0c6f48a141409df65d74a5f1dae2e139f23d50a43c58c16098/pyerfa-2.0.0.1.tar.gz (from https://pypi.org/simple/pyerfa/) (requires-python:>=3.7): Requested pyerfa>=2.0 from file:///Users/artem/Library/Caches/pip/wheels/32/ae/66/3b06f81af63921bd832dccefcb06ff1a85529a214f265e5e6a/pyerfa-0.0.0-cp311-cp311-macosx_12_0_x86_64.whl (from astropy->uio-exoplanet-group==0.1.0) has inconsistent version: expected '2.0.0.1', but metadata has '0.0.0'
```

In that case you need to add `--use-deprecated=legacy-resolver` when calling `pip install`.

#### Update a package

``` sh
$ pip install dearpygui -U
```

#### Re-install the package

``` sh
$ pip install --upgrade --force-reinstall tap-adql-sandbox
```

#### Uninstall all packages

<https://stackoverflow.com/a/11250821/1688203>

``` sh
$ pip freeze | grep -v "^-e" | cut -d "@" -f1 | xargs pip uninstall -y
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

or add that flag to `mypy.ini` (*or `setup.cfg`, if it's a package*):

``` ini
[mypy]
ignore_missing_imports = true
```

#### More strict checks

In the same `mypy.ini`/`setup.cfg`:

``` ini
[mypy]
ignore_missing_imports = True
check_untyped_defs = True
strict_optional = True
disallow_untyped_defs = True
```

### SQL database

#### Connecting to PostgreSQL database

``` py
import config
import argparse
import psycopg2

dbConnection = None
dbCursor = None

# ...

argParser.add_argument(
    "--db-username",
    required=True,
    help="Database username"
)
argParser.add_argument(
    "--db-password",
    required=True,
    help="Database password"
)

# ...

try:
    dbConnection = psycopg2.connect(
        host=config.dbHost,
        port=config.dbPort,
        user=cliArgs.db_username,
        password=cliArgs.db_password,
        database=config.dbName,
        connect_timeout=11
    )
    dbCursor = dbConnection.cursor()
    dbCursor.execute("SELECT version();")
    record = dbCursor.fetchone()
    print(f"Database connection: {record}")
except psycopg2.Error as ex:
    raise SystemExit(f"PostgreSQL error connecting to the database {ex}")
except Exception as ex:
    raise SystemExit(f"Unknown error connecting to the database {ex}")
finally:
    dbCursor.close()
```

#### Simple SELECT

``` py
try:
    dbCursor = dbConnection.cursor()
    dbCursor.execute("SELECT * FROM public.some_table;")
    records = dbCursor.fetchall()
    for row in records:
        print(f"{row[0]} - {row[1]}")
except psycopg2.Error as ex:
    print(f"[ERROR] Couldn't get data from database {ex}")
finally:
    dbCursor.close()
```

#### Parametrized SELECT

``` py
try:
    dbCursor = dbConnection.cursor()
    query = " ".join((
        f"SELECT reference_check FROM public.some_table ",
        "WHERE id = %s AND LOWER(something) LIKE %s",
        "ORDER BY another DESC LIMIT 11;"
    ))
    dbCursor.execute(
        query,
        (
            someID,
            f"{something.lower()}%"
        )
    )
    rez = dbCursor.fetchone()[0]
except psycopg2.Error as ex:
    print(f"[ERROR] Couldn't get data from database {ex}")
finally:
    dbCursor.close()
```

### Generate a random string

``` py
import string
import random

def generateRandomUsername():
    return "".join(
        random.choice(
            string.ascii_letters + string.digits
        ) for _ in range(random.randrange(11,21))
    )

for i in range(5):
    print(generateRandomUsername())
```

### SHA checksum of a file

``` py
def sha1sum(pathToFile: pathlib.Path) -> str:
    h = hashlib.sha1()
    b = bytearray(128*1024)
    mv = memoryview(b)
    with open(pathToFile, "rb", buffering=0) as f:
        while n := f.readinto(mv): # requires Python 3.8 or newer for :=
            h.update(mv[:n])
    return h.hexdigest()
```

### Virtual environment

When you need to work with some crazy package, which you don't want to pollute you main environment with, you can isolate this package in a [virtual environment](https://docs.python.org/3/library/venv.html).

``` sh
$ mkdir altaenv && cd $_
$ python -m venv .

$ ls -L1 --group-directories-first .
Include/
Lib/
Scripts/
pyvenv.cfg
```

Now it needs to be activated:

- on GNU/Linux, Mac OS: `source ./bin/activate`
- on Windows: `.\Scripts\activate.bat`

Check that list of packages is empty (*unless you told it to copy those from your main environment*):

``` sh
$ pip list
Package    Version
---------- -------
pip        22.3
setuptools 65.5.0
```

To deactivate current virtual environment you can either run `deactivate` or close the terminal session.

#### Using custom virtual environment in Visual Studio Code

In particular, for running Jupyter Notebooks with a specific Python version (*let's say, 3.9*).

Dedicate some folder on your machine exclusively for virtual environments, something like `/Users/vasya/code/python/_venvs`. Go there, create a virtual environment, activate it and install a minimum of packages that you might be using in your notebooks:

``` sh
$ cd ~/code/python/_venvs
$ mkdir caltech-workshops && cd $_
$ python3.9 -m venv .
$ source ./bin/activate

$ python --version
$ pip --version

$ pip install ipython ipykernel
$ pip install numpy pandas matplotlib
```

Open VS Code settings file and add there this line (*first check if you already have `python.venvPath` setting*):

``` json
"python.venvPath": "~/code/python/_venvs"
```

Now VS Code will able to find your virtual environments that you'll be creating in that folder going forward. As a bare minimum, it should now be able to discover the one you've just created.

Open any notebook that you have and:

1. Click on kernel indicator in the top-right corner (*or wherever you have it*);
2. It will show "*Change kernel...*" prompt, choose "*Select Another Kernel...*";
3. In the next prompt choose "*Python Environments...*";
4. In the next prompt you should have a list of your virtual environments from that folder (*likely in the bottom of the list, they will have `Virtual Env` label*).

### Set HTTP proxy

If you are using something [requests](https://github.com/psf/requests)-based, then you can set HTTP proxy via environment variable:

``` sh
HTTP_PROXY="http://127.0.0.1:4321" python ./some.py
```

If you need to set HTTPS too, then in case you are using some debugging HTTP proxy such as [Proxie](https://proxie.app/docs/tls.html) application, get that self-signed certificate file in PEM format and then:

``` sh
REQUESTS_CA_BUNDLE="/path/to/proxie-ca.pem" \
    HTTP_PROXY="http://127.0.0.1:4321" \
    HTTPS_PROXY="http://127.0.0.1:4321" \
    python ./some.py
```

If you are using some other library for sending network requests, then you will probably need a different environment variable (*such as `CURL_CA_BUNDLE`, `SSL_CERT_FILE`, something else?*).

### Closest value from a list

``` py
from bisect import bisect_left
import numpy

def findClosest(myValue, listToLookIn):
    listToLookIn = sorted(listToLookIn)
    pos = bisect_left(listToLookIn, myValue)
    if pos == 0:
        return listToLookIn[0]
    if pos == len(listToLookIn):
        return listToLookIn[-1]
    before = listToLookIn[pos - 1]
    after = listToLookIn[pos]
    if after - myValue < myValue - before:
        return after
    else:
        return before

x = 3
lst = [1, 45, 4, 15, 101, 0, 7, 3]

print("With findClosest():")
print(findClosest(x, lst)) # will print 3
print()

print("With numpy.searchsorted():")
lst = sorted(lst)
print(lst[numpy.searchsorted(lst, x)]) # will print 3
print(lst[numpy.searchsorted(lst, x, side="right")]) # will print 4
```

### Create a nested dictionary from a list of nodes

<https://stackoverflow.com/a/52349897/1688203>

``` py
import json

def splitTreeNodes(data):
    if data:
        head, *tail = data
        return {
            head: splitTreeNodes(tail)
        }
    else:
        return {}

x = splitTreeNodes(["one", "two", "three", "four"])
print(json.dumps(x, indent=4))

# {
#     "one": {
#         "two": {
#             "three": {
#                 "four": {}
#             }
#         }
#     }
# }
```

### Convert Jupyter Notebook to HTML

If you want cells results/outputs to be present too, first execute all of them and save the file. Then:

``` sh
$ jupyter nbconvert --to html /path/to/some.ipynb
```

If you did execute all the cells and did save the file, then all the outputs will be present in the resulting HTML, and the entire conversion will take about a second.

Alternatively, you can add `--execute`:

``` sh
$ jupyter nbconvert --execute --to html /path/to/some.ipynb
```

and then it will execute all the cells, even if they already have the outputs, so in case of large notebooks this might be wastful.

If you get an error like this:

``` sh
Traceback (most recent call last):
  File "/opt/homebrew/lib/python3.12/site-packages/traitlets/traitlets.py", line 632, in get
    value = obj._trait_values[self.name]
            ~~~~~~~~~~~~~~~~~^^^^^^^^^^^
KeyError: 'template_paths'

During handling of the above exception, another exception occurred:

...

ValueError: No template sub-directory with name 'lab' found in the following paths:
    /Users/vasya/Library/Jupyter
    /Users/vasya/Library/Python/3.12/share/jupyter
    /opt/homebrew/opt/python@3.12/Frameworks/Python.framework/Versions/3.12/share/jupyter
    /usr/local/share/jupyter
    /usr/share/jupyter
```

then in case of Mac OS one way to solve that (*for this particular Python version*) is:

``` sh
$ mkdir -p /opt/homebrew/opt/python@3.12/Frameworks/Python.framework/Versions/3.12/share/jupyter/nbconvert
$ cd /opt/homebrew/opt/python@3.12/Frameworks/Python.framework/Versions/3.12/share/jupyter/nbconvert
$ ln -s /opt/homebrew/share/jupyter/nbconvert/templates
```

If you need to run the conversion with a different Python version:

``` sh
$ python3.10 -m jupyter nbconvert --to html /path/to/some.ipynb
```
