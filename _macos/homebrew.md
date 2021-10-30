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
$ brew search file retriever
```

### Install package

``` sh
$ brew install wget
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
