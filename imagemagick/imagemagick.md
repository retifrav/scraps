## ImageMagick

<!-- MarkdownTOC -->

- [Supported delegates and formats](#supported-delegates-and-formats)
- [Convert PNG to JPG](#convert-png-to-jpg)
- [Resize an image](#resize-an-image)
- [Crop left side an image](#crop-left-side-an-image)

<!-- /MarkdownTOC -->

<https://imagemagick.org/>

### Supported delegates and formats

``` sh
$ magick -version
Version: ImageMagick 7.0.9-8 Q16 x86_64 2019-12-09 https://imagemagick.org
Features: Cipher DPC HDRI Modules OpenMP(3.1)
Delegates (built-in): bzlib freetype heic jng jp2 jpeg lcms ltdl lzma openexr png tiff webp xml zlib
```

``` sh
$ magick identify -list format
```

### Convert PNG to JPG

``` sh
$ magick convert -quality 90% ./some.png ./some.jpg
```

### Resize an image

Resize an image to a specific width (`100 px`) while preserving the aspect:

```
$ magick convert image.jpg -resize 100 image-resized.jpg
```

Resize all images in the current folder and append the `-thumb` suffix to files names:

```
$ for f in *; do magick convert "$f" -resize 100 "${f%.*}-thumb.${f##*.}"; done
```

### Crop left side an image

```
$ magick convert image -chop 1440x0 image-chopped.jpg
```
