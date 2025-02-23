## ImageMagick

<!-- MarkdownTOC -->

- [Supported delegates and formats](#supported-delegates-and-formats)
- [Information about files](#information-about-files)
- [Convert between formats](#convert-between-formats)
  - [PNG to JPG](#png-to-jpg)
  - [PNG to ICO](#png-to-ico)
  - [SVG to PNG](#svg-to-png)
- [Resize an image](#resize-an-image)
- [Crop left side an image](#crop-left-side-an-image)
- [Convert bunch of images](#convert-bunch-of-images)

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

### Information about files

Get information about files in the current folder using [identify](https://imagemagick.org/script/identify.php) tool:

``` sh
$ magick identify ./*
./grid-h.png PNG 920x430 920x430+0+0 8-bit sRGB 24536B 0.000u 0:00.000
./grid.png PNG 600x900 600x900+0+0 8-bit sRGB 26116B 0.000u 0:00.000
./hero.png PNG 1920x620 1920x620+0+0 8-bit sRGB 1c 239B 0.000u 0:00.000
./icon.png PNG 256x256 256x256+0+0 8-bit sRGB 19595B 0.000u 0:00.000
./logo.png PNG 4315x1024 4315x1024+0+0 8-bit sRGB 128785B 0.000u 0:00.000
```

Or a more detailed output for a particular file:

``` sh
$ magick identify -verbose ./grid-h.png
Image:
  Filename: ./grid-h.png
  Permissions: rw-r--r--
  Format: PNG (Portable Network Graphics)
  Mime type: image/png
  Class: DirectClass
  Geometry: 920x430+0+0
  Units: Undefined
  Colorspace: sRGB
  Type: TrueColor
  Endianness: Undefined
  Depth: 8-bit
  Channels: 3.0
  Channel depth:
    Red: 8-bit
    Green: 8-bit
    Blue: 8-bit
...
```

### Convert between formats

#### PNG to JPG

``` sh
$ magick convert -quality 90% ./some.png ./some.jpg
```

#### PNG to ICO

Simple case:

``` sh
$ magick image.png -background none -resize 256x256 -density 256x256 icon.ico
```

Several sizes in one:

``` sh
$ magick image.png -background none -resize 256x256 -define icon:auto-resize="256,128,96,64,48,32,16" icon.ico
```

#### SVG to PNG

``` sh
$ magick convert image.svg -background none -density 1024 -resize 1024x image.png
```

### Resize an image

Resize an image to a specific width (`100 px`) while preserving the aspect:

``` sh
$ magick convert image.jpg -resize 100 image-resized.jpg
```

Resize all images in the current folder and append the `-thumb` suffix to files names:

``` sh
$ for f in *; do magick convert "$f" -resize 100 "${f%.*}-thumb.${f##*.}"; done
```

### Crop left side an image

``` sh
$ magick convert image -chop 1440x0 image-chopped.jpg
```

### Convert bunch of images

Convert files with `main` in their name from PNG to JPG:

``` sh
$ for f in ./*-main.png; do convert -verbose -quality 50 "$f" "${f%.*}-thumb.jpg"; done
```

or just resize all the JPG files in folder:

``` sh
$ for f in ./*.jpg; do magick -verbose "$f" -resize 30% "${f%.*}.jpg"; done
```
