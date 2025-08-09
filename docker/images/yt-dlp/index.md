# yt-dlp

This is a Docker image for running [yt-dlp](https://github.com/yt-dlp/yt-dlp) to download YouTube videos, where `yt-dlp` is executed via SSH.

<!-- MarkdownTOC -->

- [Building an image](#building-an-image)
- [Creating a container](#creating-a-container)
- [Executing downloads](#executing-downloads)

<!-- /MarkdownTOC -->

## Building an image

``` sh
$ export IMAGE_NAME='yt-dlp'
$ export VER='2024.12.23'

$ docker build . -f ./Dockerfile \
    --build-arg YTDLP_VERSION_VALUE=$VER \
    --build-arg SSH_PUBLIC_KEY="$(cat ~/.ssh/ytdlp.pub)" \
    --platform linux/amd64 \
    --tag $IMAGE_NAME:$VER \
    --tag $IMAGE_NAME:latest
```

If your host will be ARM-based, skip the `--platform linux/amd64` (*or set it to `linux/arm64`*).

## Creating a container

``` yaml
version: "3"

volumes:
  ssh-host-keys:
    driver: local
    driver_opts:
      o: "bind"
      type: "none"
      device: "/PATH/TO/yt-dlp/data/ssh-host-keys"
  ssh-authorized-keys:
    driver: local
    driver_opts:
      o: "bind"
      type: "none"
      device: "/PATH/TO/yt-dlp/data/ssh-authorized-keys"

services:
  server:
    image: YOUR-REGISTRY/USERNAME/yt-dlp:latest
    container_name: yt-dlp
    restart: unless-stopped
    environment:
      - TZ=Europe/Amsterdam
    ports:
      - "2222:2222"
    volumes:
      - ssh-host-keys:/home/docker/non-root-ssh/keys
      - ssh-authorized-keys:/home/docker/.ssh
      - /PATH/TO/downloads:/home/docker/downloads
```

## Executing downloads

``` sh
$ ssh docker@YT-DLP-HOST -p 2222 -i ~/.ssh/ytdlp "screen -dmS ydl ~/ydl.sh https://youtu.be/dQw4w9WgXcQ"
```
