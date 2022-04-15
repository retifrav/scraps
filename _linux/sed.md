## sed

<!-- MarkdownTOC -->

- [Delete lines from file](#delete-lines-from-file)
- [Multiple patterns](#multiple-patterns)
- [Replace text in files](#replace-text-in-files)
- [Remove all script tags from HTML document](#remove-all-script-tags-from-html-document)

<!-- /MarkdownTOC -->

### Delete lines from file

Remove first line:

``` sh
$ sed -i "1d" some.txt
```

Remove second line:

``` sh
$ sed -i "2d" some.txt
```

Remove last line:

``` sh
$ sed -i "$d" some.txt
```

Remove lines from 2nd to 8th (including 2nd and 8th):

``` sh
$ sed -i "2,8d" some.txt
```

Remove lines containing `ololo` (regular expression):

``` sh
$ sed -i "/ololo/d" some.txt
```

### Multiple patterns

You can separate multiple patterns with `;`:

``` sh
$ sed -i '/lib\//!d;/cmake\//d;/pkgconfig\//d' /path/to/install-manifest.txt
```

here we delete all the lines that *do not* contain `lib/` or *do* contain `cmake/` or `pkgconfig/`.

Or you can use empty lines:

``` sh
$ sed "
s/ARTIFACT-NAME/artifactName/
s/ARTIFACT-ID/artifactName/
s/MAJOR-MINOR/someRelease/
s/REVISION/someRevision/
s/PACKAGE-TYPE/packageType/
" /path/to/some.template > /path/to/resulting.file
```

here we replace placeholders in some template and output resulting file.

### Replace text in files

``` sh
$ find ./ -type f -exec sed -i 's/ololo/some\/path/g' {} \;
```

* `find ./` look in the current folder
* `-type f` - apply to files
* `sed -i` - replace all the occurrences of `ololo` string with `some/path` string

### Remove all script tags from HTML document

``` sh
$ sed -i 's/<script>.*<\/script>//g;/<script>/,/<\/script>/{/<script>/!{/<\/script>/!d}};s/<script>.*//g;s/.*<\/script>//g' ./some.html
```

If `<script>` tags have attributes, then this should take case of such tags too:

``` sh
$ sed -i 's/<script.*<\/script>//g;/<script/,/<\/script>/{/<script/!{/<\/script>/!d}};s/<script.*//g;s/.*<\/script>//g' ./some.html
```
