# teamcity-server

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
