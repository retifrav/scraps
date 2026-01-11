# uv-ruff-ty

Astral Software tools: <https://astral.sh/>

<!-- MarkdownTOC -->

- [uv](#uv)
    - [Installation](#installation)
        - [Mac OS](#mac-os)
- [ruff](#ruff)
- [ty](#ty)

<!-- /MarkdownTOC -->

## uv

### Installation

#### Mac OS

``` sh
$ brew install uv
```

## ruff

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
