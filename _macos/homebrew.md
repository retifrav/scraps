## Homebrew

https://docs.brew.sh/FAQ

<!-- MarkdownTOC -->

- [Search for package](#search-for-package)
- [Install package](#install-package)
    - [Install application to custom location](#install-application-to-custom-location)
- [List of installed packages](#list-of-installed-packages)
- [Update](#update)
- [Uninstall package](#uninstall-package)
- [Cleanup](#cleanup)

<!-- /MarkdownTOC -->

### Search for package

```
$ brew search file retriever
```

### Install package

```
$ brew install wget
```

#### Install application to custom location

```
$ brew cask install --appdir="~/Applications" SomeApplication
```

Make `~/Applications` to be default installation path:

``` sh
$ nano ~/.bash_profile
export HOMEBREW_CASK_OPTS="--appdir=~/Applications"

$ source ~/.bash_profile
```

### List of installed packages

```
$ brew list
```

Only packages you installed, without dependencies:

```
$ brew leaves
```

Tree of dependencies:

```
$ brew deps --tree --installed
```

### Update

```
$ brew update
$ brew outdated
$ brew upgrade
```

### Uninstall package

```
$ brew uninstall wget
```

### Cleanup

To remove old versions of packages.

List what can be cleaned up:

```
$ brew cleanup -n
```

Cleanup particular package:

```
$ brew cleanup wget
```

Cleanup everything:

```
$ brew cleanup
```
