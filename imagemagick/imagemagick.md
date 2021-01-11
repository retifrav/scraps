## ImageMagick

- [Resize an image to certain width](#resize-an-image-to-certain-width)
- [Crop left side an image](#crop-left-side-an-image)

https://imagemagick.org/

### Resize an image to certain width

```
$ magick convert image.jpg -resize 100 image-resized.jpg
```

### Crop left side an image

```
$ magick convert image -chop 1440x0 image-chopped.jpg
```
