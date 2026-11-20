# uv-ruff-ty

Astral Software tools: <https://astral.sh/>

<!-- MarkdownTOC -->

- [uv](#uv)
    - [Installation](#installation)
        - [Mac OS](#mac-os)
        - [GNU/Linux](#gnulinux)
            - [Arch Linux](#arch-linux)
            - [Ubuntu](#ubuntu)
    - [Managing Python versions](#managing-python-versions)
    - [Virtual environments](#virtual-environments)
        - [Updating venv to a newer Python](#updating-venv-to-a-newer-python)
- [ruff](#ruff)
- [ty](#ty)

<!-- /MarkdownTOC -->

## uv

<https://docs.astral.sh/uv/>

### Installation

<https://docs.astral.sh/uv/getting-started/installation/>

#### Mac OS

``` sh
$ brew install uv
```

#### GNU/Linux

##### Arch Linux

``` sh
$ sudo pacman -S uv
```

##### Ubuntu

``` sh
$ pip install uv --break-system-packages
```

And add `~/.local/bin` to `PATH`.

### Managing Python versions

<https://docs.astral.sh/uv/concepts/python-versions/>

Current version:

``` sh
$ uv python find
/opt/homebrew/opt/python@3.14/bin/python3.14

$ python --version
Python 3.14.2
```

Currently installed versions:

``` sh
$ uv python list --only-installed
cpython-3.14.2-macos-aarch64-none     /opt/homebrew/bin/python3.14 -> ../Cellar/python@3.14/3.14.2_1/bin/python3.14
cpython-3.14.2-macos-aarch64-none     /opt/homebrew/bin/python3 -> ../Cellar/python@3.14/3.14.2_1/bin/python3
cpython-3.13.11-macos-aarch64-none    /opt/homebrew/bin/python3.13 -> ../Cellar/python@3.13/3.13.11_1/bin/python3.13
cpython-3.12.12-macos-aarch64-none    /opt/homebrew/bin/python3.12 -> ../Cellar/python@3.12/3.12.12_1/bin/python3.12
cpython-3.9.6-macos-aarch64-none      /usr/bin/python3
```

Available versions for the current platform:

``` sh
$ uv python list
cpython-3.15.0a5-macos-aarch64-none                 <download available>
cpython-3.15.0a5+freethreaded-macos-aarch64-none    <download available>
cpython-3.14.2-macos-aarch64-none                   /opt/homebrew/bin/python3.14 -> ../Cellar/python@3.14/3.14.2_1/bin/python3.14
cpython-3.14.2-macos-aarch64-none                   /opt/homebrew/bin/python3 -> ../Cellar/python@3.14/3.14.2_1/bin/python3
cpython-3.14.2-macos-aarch64-none                   <download available>
cpython-3.14.2+freethreaded-macos-aarch64-none      <download available>
cpython-3.13.11-macos-aarch64-none                  /opt/homebrew/bin/python3.13 -> ../Cellar/python@3.13/3.13.11_1/bin/python3.13
cpython-3.13.11-macos-aarch64-none                  <download available>
...
cpython-3.11.14-macos-aarch64-none                  <download available>
...
```

Installing a specific version:

``` sh
$ python3.11 --version
-bash: python3.11: command not found

$ uv python install 3.11
Installed Python 3.11.14 in 1.82s
 + cpython-3.11.14-macos-aarch64-none (python3.11)

$ uv python list --only-installed
cpython-3.14.2-macos-aarch64-none     /opt/homebrew/bin/python3.14 -> ../Cellar/python@3.14/3.14.2_1/bin/python3.14
cpython-3.14.2-macos-aarch64-none     /opt/homebrew/bin/python3 -> ../Cellar/python@3.14/3.14.2_1/bin/python3
cpython-3.13.11-macos-aarch64-none    /opt/homebrew/bin/python3.13 -> ../Cellar/python@3.13/3.13.11_1/bin/python3.13
cpython-3.12.12-macos-aarch64-none    /opt/homebrew/bin/python3.12 -> ../Cellar/python@3.12/3.12.12_1/bin/python3.12
cpython-3.11.14-macos-aarch64-none    /Users/vasya/.local/bin/python3.11 -> /Users/vasya/.local/share/uv/python/cpython-3.11.14-macos-aarch64-none/bin/python3.11
cpython-3.11.14-macos-aarch64-none    /Users/vasya/.local/share/uv/python/cpython-3.11.14-macos-aarch64-none/bin/python3.11
cpython-3.9.6-macos-aarch64-none      /usr/bin/python3

$ python3.11 --version
Python 3.11.14
```

### Virtual environments

<https://docs.astral.sh/uv/pip/environments/>

Create a new one:

``` sh
$ python --version
Python 3.14.2
$ pip --version
pip 25.3 from /opt/homebrew/lib/python3.14/site-packages/pip (python 3.14)

$ cd ~/code/python/_venvs/
$ uv venv some --python 3.11
Using CPython 3.11.14
Creating virtual environment at: some
Activate with: source some/bin/activate

$ source ./some/bin/activate
(some)

$ python --version
Python 3.11.14
(some)

$ echo $VIRTUAL_ENV
/Users/vasya/code/python/_venvs/some
(some)
```

The `pip` will be still the "global" one:

``` sh
$ pip --version
pip 25.3 from /opt/homebrew/lib/python3.14/site-packages/pip (python 3.14)
(some)
```

but that is no matter, because you actually are supposed to [install packages](https://docs.astral.sh/uv/pip/packages/) also with `uv`:

``` sh
$ uv pip list
Using Python 3.11.14 environment at: some
(some)

$ uv pip install numpy
Using Python 3.11.14 environment at: some
Resolved 1 package in 3.13s
Prepared 1 package in 529ms
Installed 1 package in 14ms
 + numpy==2.4.2
(some)

$ uv pip list
Using Python 3.11.14 environment at: some
Package Version
------- -------
numpy   2.4.2
(some)
```

#### Updating venv to a newer Python

There is no updating, you need to backup a list of installed packages, remove current environment, create a new one and restore your packages there:

``` sh
$ cd ~/code/python/_venvs/
$ source ./some/bin/activate
$ python --version
Python 3.11.14

$ uv pip freeze > ./some-requirements.txt
$ deactivate

$ uv venv --clear --python 3.15 ./some
$ source ./some/bin/activate
$ python --version
Python 3.15.0

$ uv pip install -r ./some-requirements.txt
```

If some packages don't have pre-built wheels for that version of Python on your host, they will try to build from sources, and if that fails, then it could be because your system environment C++ flags are interfering, so try again with cleared flags:

``` sh
$ LDFLAGS= CPPFLAGS= uv pip install -r ./some-requirements.txt
```

If it's all good, remove the file with the list of packages:

``` sh
$ rm ./some-requirements.txt
```

## ruff

<https://docs.astral.sh/ruff/>

Linter and formatter. Installation with [uv](#uv):

``` sh
$ uv tool install ruff@latest
```

Check a file:

``` sh
$ ruff check ./some.py
```

Check a project:

``` sh
$ cd /path/to/some/project/
$ ruff check
```

Apply fixes:

``` sh
$ ruff check --fix
```

LSP packages for integration in Sublime Text:

- [LSP-ruff](https://github.com/sublimelsp/LSP-ruff)
- [LSP-json](https://github.com/sublimelsp/LSP-json)

## ty

<https://docs.astral.sh/ty/>

Type checker. Installation with [uv](#uv):

``` sh
$ uv tool install ty@latest
```

Check a file:

``` sh
$ ty check ./some.py
```

Check a project:

``` sh
$ cd /path/to/some/project/
$ ty check
```
