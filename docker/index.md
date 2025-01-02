## Docker

My environment is Mac OS, but most of the instructions would be the same for other platforms too.

<!-- MarkdownTOC -->

- [Installation](#installation)
    - [Mac OS](#mac-os)
- [Building images](#building-images)
    - [Sample image](#sample-image)
    - [More advanced example](#more-advanced-example)
- [Deleting images](#deleting-images)
- [Exporting images](#exporting-images)
- [Publishing images](#publishing-images)
    - [Docker Hub](#docker-hub)
    - [JFrog Artifactory](#jfrog-artifactory)
        - [Authentication](#authentication)
        - [Pushing](#pushing)
    - [Gitea with self-signed certificate](#gitea-with-self-signed-certificate)
- [Creating and running a container](#creating-and-running-a-container)
    - [Attaching a console to a running container](#attaching-a-console-to-a-running-container)

<!-- /MarkdownTOC -->

### Installation

#### Mac OS

There are several ways of installing Docker on Mac OS, and first I tried to use Homebrew:

``` sh
$ brew install docker --cask
```

It might fail to symlink CLI tools to `/usr/local/bin` or wherever, but that's no worries, just add them to your `PATH` (*in `~/.bash_profile`*):

``` sh
export PATH="$PATH:$HOME/Applications/Docker.app/Contents/Resources/bin"
```

...or actually the `$HOME/.docker/bin` path, which is where it will symlink things as well.

If you'll try to install it without `--cask`, it will then install just the CLI tool, which will complain about everything with:

``` sh
Cannot connect to the Docker daemon at unix:///var/run/docker.sock. Is the docker daemon running?
```

So yeah, you need the entire fucking Docker Desktop application thing. And actually from what I remember I gave up on installing it via Homebrew, as there was something wrong with that too, and so I just download it from the [official website](https://docker.com/products/docker-desktop/).

When you'll be configuring it, there seems to be no actual need to enable settings which require admin password, such as "CLI tools installation", "Docker socket" or "Privileged port mapping".

### Building images

#### Sample image

Using an example image:

``` sh
$ git clone https://github.com/docker/welcome-to-docker.git
$ cd ./welcome-to-docker

$ docker build -t welcome-to-docker .
[+] Building 64.3s (11/11) FINISHED                                                                                                                   docker:desktop-linux
 => [internal] load .dockerignore                                                                                                                                     0.0s
 => => transferring context: 89B                                                                                                                                      0.0s
 => [internal] load build definition from Dockerfile                                                                                                                  0.0s
 => => transferring dockerfile: 684B                                                                                                                                  0.0s
 => [internal] load metadata for docker.io/library/node:18-alpine                                                                                                     5.7s
 => [1/6] FROM docker.io/library/node:18-alpine@sha256:b1a0356f7d6b86c958a06949d3db3f7fb27f95f627aa6157cb98bc65c801efa2                                               3.5s
 => => resolve docker.io/library/node:18-alpine@sha256:b1a0356f7d6b86c958a06949d3db3f7fb27f95f627aa6157cb98bc65c801efa2                                               0.0s
 => => sha256:c303524923177661067f7eb378c3dd5277088c2676ebd1cd78e68397bb80fdbf 3.35MB / 3.35MB                                                                        0.4s
 => => sha256:eb7895eb72c1a145d166631d5a81c61bffd747783c52e8a1de9fa0e7d5871817 39.98MB / 39.98MB                                                                      2.6s
 => => sha256:7049a133eb54d652049fa44657d24cef7298dc371ffab6e27f5cd5e53e62159f 2.34MB / 2.34MB                                                                        0.4s
 => => sha256:b1a0356f7d6b86c958a06949d3db3f7fb27f95f627aa6157cb98bc65c801efa2 1.43kB / 1.43kB                                                                        0.0s
 => => sha256:91f3cd192c9f1a1a907b8f53bb8cf737557e6c1bb9dc499a725e48ba6ced58fd 1.16kB / 1.16kB                                                                        0.0s
 => => sha256:7e24f77d89cc2edd3dd6963dd95bfde2dd4fc72e242b060ca583f7d93bc87efe 7.15kB / 7.15kB                                                                        0.0s
 => => extracting sha256:c303524923177661067f7eb378c3dd5277088c2676ebd1cd78e68397bb80fdbf                                                                             0.1s
 => => sha256:90577567f13c6046278cbfa4a557cb5a19ed5f9c281ab540e84ddd8f74c71922 445B / 445B                                                                            0.6s
 => => extracting sha256:eb7895eb72c1a145d166631d5a81c61bffd747783c52e8a1de9fa0e7d5871817                                                                             0.7s
 => => extracting sha256:7049a133eb54d652049fa44657d24cef7298dc371ffab6e27f5cd5e53e62159f                                                                             0.0s
 => => extracting sha256:90577567f13c6046278cbfa4a557cb5a19ed5f9c281ab540e84ddd8f74c71922                                                                             0.0s
 => [internal] load build context                                                                                                                                     0.0s
 => => transferring context: 691.83kB                                                                                                                                 0.0s
 => [2/6] WORKDIR /app                                                                                                                                                0.1s
 => [3/6] COPY package*.json ./                                                                                                                                       0.0s
 => [4/6] COPY ./src ./src                                                                                                                                            0.0s
 => [5/6] COPY ./public ./public                                                                                                                                      0.0s
 => [6/6] RUN npm install     && npm install -g serve     && npm run build     && rm -fr node_modules                                                                54.6s
 => exporting to image                                                                                                                                                0.3s
 => => exporting layers                                                                                                                                               0.3s
 => => writing image sha256:ef7e87194eaaa777fd8be8968933566aaac916437c3a0dec4b25837ac918613f                                                                          0.0s
 => => naming to docker.io/library/welcome-to-docker                                                                                                                  0.0s
```

Nowadays, there is no fucking way to find the actual file for this image in your local file system. Well, there is this path - `~/Library/Containers/com.docker.docker/Data/vms/0/` - but its contents don't look very useful. You can, however, [export it](#exporting-images).

To create and run a container from that sample image:

``` sh
$ docker run -d -p 8088:3000 --name ololo welcome-to-docker
```

and open <http://localhost:8088/> in your web-browser.

#### More advanced example

There is a more advanced/detailed example with tagging and mapping volumes [here](https://github.com/retifrav/rclone-rc-web-gui/tree/master/docker#building).

### Deleting images

``` sh
$ docker images
REPOSITORY   TAG       IMAGE ID       CREATED             SIZE
<none>       <none>    0300df560675   About an hour ago   92.4MB
alpine       latest    511a44083d3a   2 months ago        8.83MB

$ docker image rm 0300df560675
Deleted: sha256:0300df5606756b1b8d3a95de2237593703859435ebdd565b8d9a49afaf5ec6b2
```

### Exporting images

Such as for importing them into Synology DSM Container Manager afterwards:

``` sh
$ docker images
REPOSITORY          TAG                       IMAGE ID       CREATED        SIZE
rclone-rc-web-gui   latest                    0d97a5c12832   7 hours ago    92.5MB
rclone-rc-web-gui   rclone_1.68.1-gui_0.5.0   0d97a5c12832   7 hours ago    92.5MB
alpine              latest                    511a44083d3a   2 months ago   8.83MB

$ docker save rclone-rc-web-gui:latest > ./image.tar
$ echo $?
$ du -hs ./image.tar
 89M
```

### Publishing images

#### Docker Hub

If you intend to publish your image to Docker Hub, the tag should include your username, so for example instead of tagging it like this:

``` sh
--tag some:1.2.3 --tag some:latest
```

you should tag it like that:

``` sh
--tag USERNAME/some:1.2.3 --tag USERNAME/some:latest
```

And then first publish it with the "full" version tag:

``` sh
$ docker push USERNAME/rclone-rc-web-gui:1.2.3
```

followed by publishing it with the `latest` tag:

``` sh
$ docker push USERNAME/rclone-rc-web-gui:latest
```

#### JFrog Artifactory

To publish your Docker image to a self-hosted JFrog Artifactory, first create a Docker registry there (*Local Repository → Docker → simple-default → API v2*). The name can be anything, for example `docker-registry`.

##### Authentication

Authenticate with that registry from the machine, which you'll be publishing your images from. There are two ways to do that, either via CLI tool (*no need to provide `https://` here*):

``` sh
$ docker login artifactory.YOUR.HOST
```

or via manually editing `~/.docker/config.json`:

``` json
{
    "auths": {
        "artifactory.YOUR.HOST": {
            "auth": "LOGIN:APITOKEN (encoded in Base64)",
            "email": "DOESNTMATTER"
        }
    },
    "credsStore": "",
    "...": "..."
}
```

Note the empty `credsStore` property (*actually, you can just delete the entire property instead of setting it to empty value*). By default it has the value `desktop`, so apparently it stores secrets in the Docker Desktop application somewhere. Amusing enough, even though you have it set to `""`, the next run of `docker login` will still override it (*so don't run `docker login`*), although this time with a different credentials provider, such as `osxkeychain`. More amusingly, just relaunching Docker Desktop application will also override it, although this time back to original `desktop` value (*so keep an eye on that*).

The encoded value for `auth` can be composed like this:

``` sh
$ echo -n "LOGIN:APITOKEN" | base64
```

Depending on your Artifactory/registry/repository/etc, instead of the API token you might need to use your account password.

##### Pushing

First you need to tag the image:

``` sh
$ docker tag welcome-to-docker artifactory.YOUR.HOST/docker-registry/some-name:some-tag
```

And then you should be able to push it:

``` sh
$ docker push artifactory.YOUR.HOST/docker-registry/some-name:some-tag
The push refers to repository [artifactory.YOUR.HOST/docker-registry/some]
189c981deae3: Pushed
acd01806b197: Pushed
850754ae876b: Pushed
82de12acb7e4: Pushed
0d4c0e6e7a1d: Pushed
894d7d58d9b1: Pushed
682295b56738: Pushed
d606342d924f: Pushed
3ce819cc4970: Pushed
some-tag: digest: sha256:33db36b9b7c05470fad6026351814d9d344238ffd6a1d56fbfe48940a065cb9f size: 2202
```

This is the most confusing part of the whole process - to guess the right values for tagging and pushing - because fucking JFrog Artifactory's own built-in guide isn't fucking helpful, and their documentation is a wall of text about any-fucking-thing but the actual pushing command.

So, just in case, if your full URL to the registry reported by Artifactory in its UI is `https://artifactory.YOUR.HOST:443/artifactory/docker-registry/`, then:

- the registry "address" for `docker tag` and `docker push` commands is `artifactory.YOUR.HOST/docker-registry`;
- `some-name` is the name that your image should have in the registry, so you just choose whatever;
- `some-tag` is the tag you'd like to assign to it, as images can have several tags, and usually those are used as versions.

#### Gitea with self-signed certificate

If trying to log-in your get something like:

```
Error response from daemon: Get "https://192.168.1.111:12345/v2/": tls: failed to verify certificate: x509: cannot validate certificate for 192.168.1.111 because it doesn't contain any IP SANs
```

Then add your Gitea host to `insecure-registries` in `~/.docker/daemon.json`:

``` json
{
    "builder":
    {
        "gc":
        {
            "defaultKeepStorage": "20GB",
            "enabled": true
        }
    },
    "experimental": false,
    "insecure-registries": ["192.168.1.111:12345"]
}
```

Create an Access Token with `write:package` and `write:repository` (*might be redundant*) permissions, use that token as the password on logging in:

``` sh
$ docker login 192.168.1.111:12345
Username: YOUR-USERNAME
Password:
WARNING! Your password will be stored unencrypted in /Users/vasya/.docker/config.json.
Configure a credential helper to remove this warning. See
https://docs.docker.com/engine/reference/commandline/login/#credential-stores

Login Succeeded
```

After that you will be able to (*tag and*) push your images to that registry:

``` sh
$ docker images
REPOSITORY                                TAG                       IMAGE ID       CREATED        SIZE
decovar/teamcity-agent                    2024.12                   1c00f14b9868   14 hours ago   2.52GB
decovar/teamcity-agent                    latest                    1c00f14b9868   14 hours ago   2.52GB
decovar/teamcity-server                   2024.12                   c63e74bb2c6e   43 hours ago   3.53GB
decovar/teamcity-server                   latest                    c63e74bb2c6e   43 hours ago   3.53GB
decovar/rclone-rc-web-gui                 latest                    ebcefa21a859   5 weeks ago    91.4MB
decovar/rclone-rc-web-gui                 rclone_1.68.1-gui_0.5.0   ebcefa21a859   5 weeks ago    91.4MB
alpine                                    latest                    511a44083d3a   3 months ago   8.83MB

$ docker tag decovar/teamcity-agent 192.168.1.111:12345/YOUR-USERNAME/teamcity-agent:latest
$ docker push 192.168.1.111:12345/YOUR-USERNAME/teamcity-agent:latest
```

### Creating and running a container

``` sh
$ docker run -it --rm IMAGE_ID /bin/ash
```

A more detailed example [here](https://github.com/retifrav/rclone-rc-web-gui/blob/master/docker/README.md#running-a-container).

#### Attaching a console to a running container

``` sh
$ docker exec -it CONTAINER_ID bash
```

If the container image does not have `bash`, try other shells, such as `ash` or simply `sh`.
