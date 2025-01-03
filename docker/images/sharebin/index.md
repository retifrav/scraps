# teamcity-agent

This is a Docker image for [sharebin](https://github.com/Timshel/sharebin).

It is 1-to-1 the same as the [official image](https://hub.docker.com/r/timshel/sharebin), the only difference is additional commands to allow running it with non-root user, so the image could be used in [Container Manager](https://synology.com/en-global/dsm/feature/docker) on Synology DSM.

Pre-built image is published on [Docker Hub](https://hub.docker.com/r/decovar/sharebin).

## Building

``` sh
$ export IMAGE_NAME='sharebin'
$ export VER='2.0.5'

$ docker build . -f ./Dockerfile \
    --build-arg VER=$VER \
    --platform linux/amd64 \
    --tag $IMAGE_NAME:$VER \
    --tag $IMAGE_NAME:latest

$ docker images
REPOSITORY                                TAG                       IMAGE ID       CREATED         SIZE
sharebin                          2.0.5                     2564ae818e9c   5 seconds ago   89.9MB
sharebin                          latest                    2564ae818e9c   5 seconds ago   89.9MB
```
