## Kaleidoscope

<!-- MarkdownTOC -->

- [Compare files](#compare-files)
- [Preview sed results](#preview-sed-results)
- [Watch CPU usage](#watch-cpu-usage)

<!-- /MarkdownTOC -->

### Compare files

``` sh
$ ksdiff /path/to/file1.txt /path/to/file2.txt
```

### Preview sed results

``` sh
$ cat some.txt | sed "s/some/another/g" | ksdiff some.txt -
```

### Watch CPU usage

``` sh
$ ps -crx -o%cpu,command | grep -e "^\s.[1-9]" | ksdiff -l "ps"
```
