## Homebrew

<!-- MarkdownTOC -->

- [Homebrew](#homebrew)
    - [Search for package](#search-for-package)
    - [Install package](#install-package)
    - [List of installed packages](#list-of-installed-packages)
    - [Update](#update)
    - [Uninstall package](#uninstall-package)
    - [Cleanup](#cleanup)

<!-- /MarkdownTOC -->

### Homebrew

https://docs.brew.sh/FAQ

#### Search for package

```
$ brew search file retriever
```

#### Install package

```
brew install wget
```

#### List of installed packages

```
brew list
```

Only packages you installed, without dependencies:

```
brew leaves
```

Tree of dependencies:

```
brew deps --tree --installed
```

#### Update

```
brew update
brew outdated
brew upgrade
```

#### Uninstall package

```
brew uninstall wget
```

#### Cleanup

To remove old versions of packages.

List what can be cleaned up:

```
brew cleanup -n
```

Cleanup particular package:

```
brew cleanup wget
```

Cleanup everything:

```
brew cleanup
```
