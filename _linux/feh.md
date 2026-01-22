# feh

<!-- MarkdownTOC -->

- [etc](#etc)
- [Actions](#actions)
- [Saving files](#saving-files)

<!-- /MarkdownTOC -->

## etc

- [Viewing images with certain geometry](https://github.com/retifrav/scraps/blob/master/_linux/index.md#find-images-with-certain-geometry-and-pass-the-list-to-feh)

## Actions

Copying files, symlinking files, etc:

``` sh
$ nano ~/.bashrc
```
``` sh
alias feh-copy="feh --action 'cp \"%f\" ~/Pictures/'"
alias feh-symlink="feh --action 'ln -s \"%f\" ~/Pictures/_wallpapers/'"
```

And then pressing `0` will execute the action. It is possible to bind different actions to different numbers, but here it's whichever alias was used.

## Saving files

Launch it with `-j`:

``` sh
$ mkdir -p ~/Pictures/new
$ feh . -j ~/Pictures/new/
```

and save files with `s`. If you won't provide a path with `-j`, then it will be saving to the same current folder but of course with renaming a copy.

Note that it loses some EXIF information and apparently re-encodes files, because they are getting smaller, which is apparently a [bug](https://github.com/derf/feh/issues/524) (*or at least some crazy default setting that is nowhere to find*).
