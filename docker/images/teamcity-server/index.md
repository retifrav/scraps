# teamcity-server

This is a Docker image for [TeamCity](https://jetbrains.com/teamcity/).

It is 1-to-1 the same as the [official image](https://hub.docker.com/r/jetbrains/teamcity-server), the only difference is additional commands to "unhardcode" the `UID`/`GID` values, so the image could be used in [Container Manager](https://synology.com/en-global/dsm/feature/docker) on Synology DSM, as described in [this bugreport](https://youtrack.jetbrains.com/issue/BND-1756/Cannot-use-Docker-Image-on-Synology-NAS-as-UID-GID-is-static#focus=Comments-27-6921347.0-0).

Pre-built image is published on [Docker Hub](https://hub.docker.com/r/decovar/teamcity-server).

## Building

``` sh
$ export IMAGE_NAME='teamcity-server'
$ export VER='2024.12'

$ docker build . -f ./Dockerfile \
    --build-arg VER=$VER \
    --platform linux/amd64 \
    --tag $IMAGE_NAME:$VER \
    --tag $IMAGE_NAME:latest

$ docker images
REPOSITORY                  TAG                       IMAGE ID       CREATED          SIZE
teamcity-server             2024.12                   2303fb8f67f1   14 seconds ago   3.53GB
teamcity-server             latest                    2303fb8f67f1   14 seconds ago   3.53GB
```
