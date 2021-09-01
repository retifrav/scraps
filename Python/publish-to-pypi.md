## Publish a script as a command line tool to PyPI

Main tutorial: <https://packaging.python.org/tutorials/packaging-projects/>

<!-- MarkdownTOC -->

- [Project](#project)
    - [Structure](#structure)
    - [__init__.py](#__init__py)
    - [example.py](#examplepy)
    - [pyproject.toml](#pyprojecttoml)
    - [setup.cfg](#setupcfg)
- [Publishing](#publishing)

<!-- /MarkdownTOC -->

### Project

#### Structure

```
$ tree .
.
├── pyproject.toml
├── setup.cfg
└── src
    └── example_package
        ├── __init__.py
        └── example.py
```

#### __init__.py

Empty file.

#### example.py

``` py
def add_one(number):
    return number + 1


def main():
    print(f"Result: {add_one(1)}")
    raise SystemExit(0)
```

#### pyproject.toml

```
[build-system]
requires = [
    "setuptools>=42",
    "wheel"
]
build-backend = "setuptools.build_meta"
```

#### setup.cfg

```
[metadata]
name = example-pkg-YOURNAME
version = 0.0.1
author = YOURNAME
author_email = PUBLIC-EMAIL
description = A small example package
long_description = file: README.md
long_description_content_type = text/markdown
keywords = one, two
url = https://github.com/pypa/sampleproject
project_urls =
    Bug Tracker = https://github.com/pypa/sampleproject/issues
classifiers =
    Programming Language :: Python :: 3.7
    License :: OSI Approved :: GNU General Public License v3 (GPLv3)
    Operating System :: MacOS
    Intended Audience :: Developers
    Topic :: Multimedia :: Graphics :: Graphics Conversion

[options]
package_dir =
    = src
packages = find:
python_requires = >=3.7

[options.packages.find]
where = src

[options.entry_points]
console_scripts =
    plusone = example_package.example:main
```

### Publishing

Create an account at [PyPI](https://pypi.org/), or actually first at [TestPyPI](https://test.pypi.org/) for experimenting.

Build your package:

``` sh
$ python -m build
```

Install `twine`:

``` sh
$ pip install twine
```

Create a config for it:

```
$ nano ~/.pypirc
[testpypi]
username = __token__
password = pypi-YOUR-TOKEN
```

Upload your package:

``` sh
$ twine upload --repository testpypi dist/*
```

Install your package and test it:

``` sh
$ pip install -i https://test.pypi.org/simple/ example-pkg-YOURNAME --upgrade

$ plusone
Result: 2

$ echo $?
0
```
