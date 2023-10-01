## Homebrew

https://docs.brew.sh/FAQ

<!-- MarkdownTOC -->

- [Search for package](#search-for-package)
- [Install package](#install-package)
    - [Install application to custom location](#install-application-to-custom-location)
- [List installed packages](#list-installed-packages)
- [Update](#update)
- [Uninstall package](#uninstall-package)
- [Cleanup](#cleanup)

<!-- /MarkdownTOC -->

### Search for package

``` sh
$ brew search python
```

Information about known package:

``` sh
$ brew info python
```

Available versions:

``` sh
$ brew info --json python | jq -r '.[].versioned_formulae[]'
python@3.10
python@3.9
python@3.8
python@3.7
```

### Install package

``` sh
$ brew install protobuf
```

Specific version:

``` sh
$ brew install protobuf@21
```

If it is a keg-only, but you want it in the main Homebrew prefix, then you can link it:

``` sh
$ brew link protobuf@21
```

#### Install application to custom location

``` sh
$ brew cask install --appdir="~/Applications" SomeApplication
```

Make `~/Applications` to be default installation path:

``` sh
$ nano ~/.bash_profile
export HOMEBREW_CASK_OPTS="--appdir=~/Applications"

$ source ~/.bash_profile
```

### List installed packages

``` sh
$ brew list
```

Only packages you installed, without dependencies:

``` sh
$ brew leaves
```

Tree of dependencies:

``` sh
$ brew deps --tree --installed
```

Those that depend on a specific package:

``` sh
$ brew uses --recursive --installed qt@5
```

### Update

``` sh
$ brew update
$ brew outdated
$ brew upgrade
```

### Uninstall package

``` sh
$ brew uninstall wget
```

### Cleanup

To remove old versions of packages.

List what can be cleaned up:

``` sh
$ brew cleanup -n
```

Cleanup particular package:

``` sh
$ brew cleanup wget
```

Cleanup everything:

``` sh
$ brew cleanup
```
