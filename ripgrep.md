# ripgrep

- <https://github.com/burntsushi/ripgrep>
- <https://codapi.org/try/ripgrep/>

A better/faster grepping tool.

<!-- MarkdownTOC -->

- [Find zlib in vcpkg manifests excluding certain ports](#find-zlib-in-vcpkg-manifests-excluding-certain-ports)

<!-- /MarkdownTOC -->

## Find zlib in vcpkg manifests excluding certain ports

``` sh
$ cd /path/to/vcpkg-registry/ports/
$ rg -l \
    -F '"zlib"' \
    -g 'vcpkg.json' \
    -g '!mesa/vcpkg.json' \
    -g '!civetweb/vcpkg.json'
```

where:

- `-l` - print out only the file names;
- `-F` - literal match for the double quotes;
- `-g` - recursive search for the specified file name to look in, where `!` is for excluding certain files.
