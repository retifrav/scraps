## Publish a script as a command line tool to PyPI

Tutorials:

- <https://packaging.python.org/tutorials/packaging-projects/>
- <https://python-packaging-tutorial.readthedocs.io/en/latest/setup_py.html>

<!-- MarkdownTOC -->

- [Project](#project)
    - [`__init__.py`](#__init__py)
    - [`example.py`](#examplepy)
    - [`pyproject.toml`](#pyprojecttoml)
    - [`setup.cfg`](#setupcfg)
- [Running locally](#running-locally)
- [Publishing](#publishing)

<!-- /MarkdownTOC -->

### Project

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

#### `__init__.py`

Empty file.

#### `example.py`

``` py
def add_one(number):
    return number + 1


def main():
    print(f"Result: {add_one(1)}")
    raise SystemExit(0)


if __name__ == '__main__':
    main()
```

#### `pyproject.toml`

```
[build-system]
requires = [
    "setuptools>=42",
    "wheel"
]
build-backend = "setuptools.build_meta"
```

#### `setup.cfg`

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

### Running locally

If you'd like to use/run you module/application without installing it, you can install it in a special development/editable way:

``` sh
$ cd /path/to/your/project/repository
$ tree .
.
├── pyproject.toml
├── setup.cfg
└── src
    └── example_package
        ├── __init__.py
        └── __main__.py

$ pip install -e ./
```

Here `__main__.py` is actually the very same [example.py](#examplepy), but if you'd like to be able to run your module as an application, it needs to have `__main__.py`.

Now you can run your module/application like this:

``` sh
$ python -m example_package
```

If you make any changes in the sources, there is no need to run `pip install -e ./` - your module already has them.

When you're done with local testing, uninstall the package:

``` sh
$ pip uninstall example_package
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
