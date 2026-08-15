# a JavaScript engine is required now, thanks to YouTube
# https://github.com/yt-dlp/yt-dlp/wiki/EJS
#
# the base image ships a patched `tini` at `/tini` (which is used as PID 1),
# so there is no need to install Alpine's own `tini` package
#FROM denoland/deno:alpine-2.6.7
FROM denoland/deno:alpine-2.9.0

# this argument is provided by BuildKit, it has to be re-declared to be visible
#
# referenced as `${VAR:-}` so that a build without BuildKit would fail with a clear error
# instead of tripping over `set -u`
ARG TARGETARCH

# an escape hatch (`--build-arg SKIP_SMOKE_TESTS=1`) for the rare case when
# the target binaries do run under emulation, but misbehave under it
#
# the `FROM` above is not pinned to `$BUILDPLATFORM` (and there is only one stage anyway),
# so on a cross-build every RUN is emulated, which means that if `apk add` below has got
# that far, then the emulator can run the target binaries too, and the smoke tests are just
# as meaningful as they are in a native build
ARG SKIP_SMOKE_TESTS=""

ARG YTDLP_VERSION_VALUE="2026.07.04"
# for nightly builds it would be `yt-dlp-nightly-builds`
ARG YTDLP_DOWNLOAD_TYPE="yt-dlp"
# when this one is empty, it is derived from TARGETARCH, so you should only override it
# for a non-musl base images, so that download names would be `yt-dlp_linux`/`yt-dlp_linux_aarch64`
ARG YTDLP_DOWNLOAD_NAME=""

ARG UID=1000
ARG USERNAME=docker

ARG GID=1000
ARG GROUPNAME=dockers

ARG SSH_PORT=2222

# for `org.opencontainers.image.revision`, which has to be passed in, because a LABEL value
# can only come from an ARG/ENV or a literal - there is no Dockerfile syntax for capturing
# the output of something like `git rev-parse HEAD`, not even via a multi-stage build
#
# letting `.git` into the build context would not change that, and it is not in the context
# to begin with, because the context is this current directory, but `.git` lives at the root
# level of the repository
ARG VCS_REF=""

# these have to be ENV and not ARG, because the ENTRYPOINT script needs them at runtime,
# as they describe the image layout and are not meant to be overridden with `docker run -e`
ENV NON_ROOT_SSH_FOLDER="/home/$USERNAME/non-root-ssh"
# `DENO_DIR` in the base image is `/deno-dir/` and is owned by the `deno` user (1000:1000),
# so it is unwritable for our image user, which would leave EJS with a broken Deno cache
ENV DENO_DIR="/home/$USERNAME/.cache/deno"

# `--no-cache` already fetches a fresh index, so `--update` would be redundant
#
# the `supercronic` package is now in Alpine's `community` repository (v3.21 onwards),
# so pulling it from `edge` (and permanently adding an edge repository to /etc/apk/repositories)
# is not needed anymore
RUN apk add --no-cache \
        ffmpeg \
        gettext \
        nano \
        openssh \
        python3 \
        screen \
        su-exec \
        supercronic \
        tzdata
        #
        #lsof \
        #openrc

# as usual, the checksum file is served from the same host as the binary,
# so we can only use it for verifying that the download went fine
RUN set -eu; \
    asset="$YTDLP_DOWNLOAD_NAME"; \
    if [ -z "$asset" ]; then \
        case "${TARGETARCH:-}" in \
            amd64) asset="yt-dlp_musllinux" ;; \
            arm64) asset="yt-dlp_musllinux_aarch64" ;; \
            *) echo "no musl yt-dlp asset is known for TARGETARCH='${TARGETARCH:-}'" >&2; exit 1 ;; \
        esac; \
    fi; \
    releases="https://github.com/yt-dlp/$YTDLP_DOWNLOAD_TYPE/releases/download/$YTDLP_VERSION_VALUE"; \
    cd /tmp; \
    wget -q -O "$asset" "$releases/$asset"; \
    wget -q -O SHA2-256SUMS "$releases/SHA2-256SUMS"; \
    expected="$(awk -v want="$asset" '$2 == want { print $1 }' SHA2-256SUMS)"; \
    [ -n "$expected" ] || { echo "no checksum published for '$asset'" >&2; exit 1; }; \
    actual="$(sha256sum "$asset")"; \
    actual="${actual%% *}"; \
    [ "$expected" = "$actual" ] || { echo "checksum mismatch for '$asset': got $actual, expected $expected" >&2; exit 1; }; \
    install -m 0755 "$asset" /usr/bin/yt-dlp; \
    rm -f "$asset" SHA2-256SUMS; \
    if [ -z "${SKIP_SMOKE_TESTS:-}" ]; then \
        yt-dlp --version; \
    else \
        echo "SKIP_SMOKE_TESTS is set, skipping the yt-dlp smoke test"; \
    fi

# this should go after the packages and yt-dlp, so that changing UID/GID would not invalidate
# those (more expensive) layers
#
# the base image creates its own `deno` user and group exactly as `1000:1000`,
# and busybox's `adduser`/`addgroup` refuses to reuse an ID that is taken - it simply
# dies with "uid '1000' in use", and there is no `--non-unique` option like `-o` in `shadow`,
# so since that account is not used here (`DENO_DIR` is overridden, `/deno-dir` is not used
# either), it is removed to free up the default `1000:1000`
#
# the `chown -R` is "free" here because it shares a layer with the `mkdir` that created
# the files (as a separate RUN it would copy the whole tree again)
#
# the Deno check runs as the image user, unlike a bare `deno --version` as root,
# so it also tests that `DENO_DIR` override
RUN set -eu; \
    echo "Removing the base image unused deno account"; \
    deluser --remove-home deno 2>/dev/null || true; \
    delgroup deno 2>/dev/null || true; \
    rm -rf /deno-dir; \
    echo "Creating the image user/group with UID $UID and GID $GID"; \
    addgroup \
        --gid "$GID" \
        "$GROUPNAME"; \
    adduser \
        --disabled-password \
        --gecos "" \
        --shell /bin/ash \
        --ingroup "$GROUPNAME" \
        --uid "$UID" \
        "$USERNAME"; \
    mkdir -p \
        "/home/$USERNAME/downloads" \
        "/home/$USERNAME/.config/yt-dlp" \
        "/home/$USERNAME/.ssh" \
        "$DENO_DIR" \
        "$NON_ROOT_SSH_FOLDER/keys"; \
    chmod 0700 "/home/$USERNAME/.ssh"; \
    chown -R "$USERNAME:$GROUPNAME" "/home/$USERNAME"; \
    if [ -z "${SKIP_SMOKE_TESTS:-}" ]; then \
        printf 'console.log("deno cache is writable")\n' > /tmp/deno-check.js; \
        su-exec "$USERNAME" deno run --quiet /tmp/deno-check.js; \
        rm -f /tmp/deno-check.js; \
    else \
        echo "SKIP_SMOKE_TESTS is set, skipping the deno smoke test"; \
    fi

COPY --chown=$USERNAME:$GROUPNAME ./config /home/$USERNAME/.config/yt-dlp/
COPY --chown=$USERNAME:$GROUPNAME ./ydl.sh /home/$USERNAME/

COPY ./kinocheck-trailers.py /usr/bin/
COPY ./crontab /etc/supercronic/crontab

# rendered into $NON_ROOT_SSH_FOLDER by the ENTRYPOINT script, so that
# SSH_PORT stays a runtime setting instead of being frozen into the image
COPY ./sshd_config.template /usr/local/share/yt-dlp/

COPY --chmod=0755 ./entrypoint.sh ./healthcheck.sh /usr/local/bin/

WORKDIR /home/$USERNAME

# PUID/PGID lets a user match the ownership of the mapped paths without rebuilding
# the image. The ENTRYPOINT script reconciles the account and then drops privileges
ENV USERNAME="$USERNAME" \
    GROUPNAME="$GROUPNAME" \
    PUID="$UID" \
    PGID="$GID" \
    SSH_PORT="$SSH_PORT" \
    ENABLE_CRON="1" \
    CRON_PIDFILE="/tmp/supercronic.pid"

EXPOSE $SSH_PORT

# sshd dying takes the container down on its own, and this exists to also
# catch supercronic dying, which would otherwise go unnoticed
HEALTHCHECK --interval=30s --timeout=5s --start-period=15s --retries=3 \
    CMD ["/usr/local/bin/healthcheck.sh"]

# overriding the OCI labels inherited from the base image,
# which would otherwise advertise this image as Deno
#
# `source` is the repository root rather than the subdirectory these files live in,
# because it is meant to be a clonable URL (it is the successor of Label Schema's
# `vcs-url`), and because GHCR only recognizes the `https://github.com/OWNER/REPO`
# form when linking a package to its repository - the subdirectory goes into `url`
#
# `documentation` points at the file directly, since GitHub only auto-renders
# `README*` in a directory listing, so `index.md` would go unnoticed under `url`
#
# `licenses` is the license of the files in this repository, the third party software
# that the image bundles (FFmpeg, OpenSSH, Deno, Python, busybox, yt-dlp) comes under
# their own separate terms
LABEL org.opencontainers.image.title="yt-dlp" \
      org.opencontainers.image.description="yt-dlp with FFmpeg, controlled over SSH and with a movie trailers cron job" \
      org.opencontainers.image.version="$YTDLP_VERSION_VALUE" \
      org.opencontainers.image.revision="$VCS_REF" \
      org.opencontainers.image.licenses="GPL-3.0-or-later" \
      org.opencontainers.image.source="https://github.com/retifrav/scraps" \
      org.opencontainers.image.url="https://github.com/retifrav/scraps/tree/master/docker/images/yt-dlp" \
      org.opencontainers.image.documentation="https://github.com/retifrav/scraps/blob/master/docker/images/yt-dlp/index.md"

# `/tini` comes from the base image, ,aking it PID 1 with `-g` forwards SIGTERM/SIGINT
# to the entire process group, which lets both sshd and supercronic shut down gracefully
#
# there is no `USER` instruction: entrypoint.sh starts as root (so it can honour PUID/PGID
# and generate SSH host keys) and then drops to $USERNAME with `su-exec` before anything
# listens on the network. It runs sshd when given no arguments, so `docker run <image> sh`
# still provides a shell
ENTRYPOINT [ "/tini", "-g", "--", "/usr/local/bin/entrypoint.sh" ]
