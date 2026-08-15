# yt-dlp

This is a Docker image for running [yt-dlp](https://github.com/yt-dlp/yt-dlp) to download YouTube videos, where `yt-dlp` is executed via SSH.

<!-- MarkdownTOC -->

- [Building an image](#building-an-image)
    - [Multi-architecture builds](#multi-architecture-builds)
    - [A cron job for downloading movies trailers](#a-cron-job-for-downloading-movies-trailers)
- [Creating a container](#creating-a-container)
    - [SSH host keys and authorized keys](#ssh-host-keys-and-authorized-keys)
    - [Running as a different UID/GID](#running-as-a-different-uidgid)
    - [Health](#health)
- [Executing downloads](#executing-downloads)

<!-- /MarkdownTOC -->

## Building an image

``` sh
$ export IMAGE_NAME='yt-dlp'
$ export VER='2026.07.04'

$ docker build . -f ./Dockerfile \
    --build-arg YTDLP_VERSION_VALUE=$VER \
    --build-arg VCS_REF="$(git rev-list -1 HEAD -- .)" \
    --tag $IMAGE_NAME:$VER \
    --tag $IMAGE_NAME:latest
```

For getting [nightly builds](https://github.com/yt-dlp/yt-dlp-nightly-builds/) you'll also need to provide `--build-arg YTDLP_DOWNLOAD_TYPE=yt-dlp-nightly-builds`.

The `VCS_REF` ends up in `org.opencontainers.image.revision`. It has to be passed in from the outside, because a `LABEL` value can only come from a build argument, an environment variable or a literal, since there is no `Dockerfile` syntax for capturing the output of a command such as `git rev-parse HEAD`. The `git rev-list -1 HEAD -- .` is the last commit that touched this directory, so the label does not change every time something unrelated is committed elsewhere.

The SSH public key is not a build argument anymore, because `/home/docker/.ssh` is normally a mounted volume, and anything baked into the image at that path is shadowed by the mount. Pass it at [runtime](#ssh-host-keys-and-authorized-keys) instead.

### Multi-architecture builds

The appropriate yt-dlp asset (*`yt-dlp_musllinux` or `yt-dlp_musllinux_aarch64`*) is picked from BuildKit's `TARGETARCH`, so a plain `docker build` produces a working image on both `amd64` and `arm64` hosts and no `--platform` is needed.

To build for a different architecture than the host, add the `--platform linux/arm64` argument (*replace `linux/arm64` with yours*).

The `yt-dlp --version` and `deno run` smoke tests run in cross-builds too. Since this `Dockerfile` has a single stage and its `FROM` is not pinned to `$BUILDPLATFORM`, every `RUN` is emulated on a cross-build, so if the build got as far as installing the packages, then the emulator can run the target binaries just as well. In other words, building `linux/amd64` on an Apple Silicon Mac should be able to test the binaries just fine.

To skip the tests you can add `--build-arg SKIP_SMOKE_TESTS=1`.

### A cron job for downloading movies trailers

The image has a cron job that runs `kinocheck-trailers.py` script to download new movies trailers from [KinoCheck](https://api.kinocheck.com/trailers/latest?language=en). It is a runtime setting, so you can set `ENABLE_CRON=0` environment variable to disable it.

## Creating a container

``` yaml
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
      # should match the owner of the downloads directory on the host
      - PUID=1000
      - PGID=1000
      - SSH_PORT=2222
      - ENABLE_CRON=1
      - "SSH_PUBLIC_KEY=ssh-ed25519 AAAAC3Nz... YOU@YOUR-HOST"
    ports:
      - "2222:2222"
    volumes:
      - ssh-host-keys:/home/docker/non-root-ssh/keys
      - ssh-authorized-keys:/home/docker/.ssh
      - /PATH/TO/downloads:/home/docker/downloads
```

The container starts as `root` on purpose and drops to the unprivileged user with `su-exec` before `sshd` or `supercronic` are started, which is what makes `PUID`/`PGID` work. Because of that it cannot be run under a `runAsNonRoot` policy, and `docker run --user ...` disables the account setup.

### SSH host keys and authorized keys

The host keys are generated into `$NON_ROOT_SSH_FOLDER/keys`, if that directory does not contain them yet. In older revisions they were generated during the build, which meant everyone who pulled the image shared the same private host key, and the bind-mounted volume was hiding it anyway, so `sshd` had no host key at all on a fresh host directory.

The `authorized_keys` is created from the `SSH_PUBLIC_KEY` environment variable - appended only if that exact line is not already present, so it is safe to keep it across restarts. You can also just drop an `authorized_keys` file into the mounted directory and leave `SSH_PUBLIC_KEY` unset. If neither is present, the entrypoint will print a warning that nobody will be able to log in.

Since the host keys are now generated per deployment, the first connection after switching to this image will report a changed host key. You can remove the stale entry with `ssh-keygen -R '[YT-DLP-HOST]:2222'`.

### Running as a different UID/GID

`PUID`/`PGID` defaults to the `UID`/`GID` build arguments, which are `1000`/`1000` — the IDs a single-user Linux host assigns first, so on most setups the mounted downloads directory already has a proper ownership and nothing has to be changed. When they differ from the baked-in values, the entrypoint recreates the user account with the requested IDs and fixes the ownership of the affected folders.

For the downloads directory only the mount point itself is chowned, and only when it is actually wrong, because a recursive chown would walk the entire (*potentially large*) folder on every start. If you change `PUID`/`PGID` after the downloads folder already got created, you'll need to chown the host directory.

Also worth to mention that the base image ships its own `deno` user at exactly `1000:1000`, and busybox `adduser`/`addgroup` refuse to reuse a taken ID, so the build removes that unused account together with the `/deno-dir` it owned, so `1000:1000` becomes available. For the same reason a `PUID`/`PGID` that belongs to some other account is rejected at startup instead of being silently ignored.

### Health

The image has a `HEALTHCHECK` that verifies that `sshd` is alive and (*unless `ENABLE_CRON=0` is set*) that `supercronic` is alive as well. The `sshd` exiting takes the container down by itself, but `supercronic` could die unnoticed, leaving the container without running cron jobs (*again, if those were enabled at all*).

``` sh
$ docker inspect --format '{{.State.Health.Status}}' yt-dlp
```

The `sshd` logs to `stderr` (*`-e`*), so authentication attempts and failures will show up in `docker logs yt-dlp` instead of disappearing into a syslog daemon that does not exist in the container.

## Executing downloads

``` sh
$ ssh docker@YT-DLP-HOST -p 2222 -i ~/.ssh/ytdlp "screen -dmS ydl ~/ydl.sh https://youtu.be/dQw4w9WgXcQ"
```
