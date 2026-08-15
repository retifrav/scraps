#!/bin/sh

# Running as root on continer initialization so that it can:
#
# 1. Reconcile the application account with PUID/PGID, so the ownership
#    of the mapped paths could be matched without rebuilding the image;
# 2. Seed `authorized_keys` from `$SSH_PUBLIC_KEY` and generates SSH host
#    keys if the mounted volumes do not have any yet;
# 3. Render `sshd_config`, so `$SSH_PORT` is a runtime setting;
# 4. Start `supercronic`, drop privileges and then `exec` the main command.

set -eu

home="/home/$USERNAME"
keys="$NON_ROOT_SSH_FOLDER/keys"
sshd_config="$NON_ROOT_SSH_FOLDER/sshd_config"

PUID="${PUID:-$(id -u "$USERNAME")}"
PGID="${PGID:-$(id -g "$USERNAME")}"

log() {
    echo "[entrypoint] $*"
}

is_root() {
    [ "$(id -u)" -eq 0 ]
}

require_uint() {
    case "$2" in
        '' | *[!0-9]*)
            log "$1 must be a non-negative integer, got '$2'"
            exit 1
            ;;
    esac
}

# the account is re-created, not modified with `usermod -u`, because `usermod`
# recursively chowns `$HOME`, which would walk the entire (potentially very large)
# downloads mount on every container start
remap_account() {
    require_uint PUID "$PUID"
    require_uint PGID "$PGID"

    currentUID="$(id -u "$USERNAME")"
    currentGID="$(id -g "$USERNAME")"

    if [ "$currentUID" = "$PUID" ] && [ "$currentGID" = "$PGID" ]; then
        return 0
    fi

    # busybox `adduser`/`addgroup` has no `--non-unique` option and dies with
    # "uid 'N' in use" if the ID belongs to another account, so instead we report
    # the collision (`65534` (`nobody`/`nogroup`) is the one to likely fail

    conflictingUser="$(awk -F: -v id="$PUID" -v self="$USERNAME" \
        '$3 == id && $1 != self { print $1; exit }' /etc/passwd)"
    if [ -n "$conflictingUser" ]; then
        log "cannot use PUID=$PUID, it already belongs to the '$conflictingUser' account"
        exit 1
    fi

    conflictingGroup="$(awk -F: -v id="$PGID" -v self="$GROUPNAME" \
        '$3 == id && $1 != self { print $1; exit }' /etc/group)"
    if [ -n "$conflictingGroup" ]; then
        log "cannot use PGID=$PGID, it already belongs to the '$conflictingGroup' group"
        exit 1
    fi

    log "remapping $USERNAME from $currentUID:$currentGID to $PUID:$PGID"
    deluser "$USERNAME" 2>/dev/null || true
    delgroup "$GROUPNAME" 2>/dev/null || true
    addgroup --gid "$PGID" "$GROUPNAME"
    adduser \
        --disabled-password \
        --gecos "" \
        --shell /bin/ash \
        --ingroup "$GROUPNAME" \
        --uid "$PUID" \
        --home "$home" \
        --no-create-home \
        "$USERNAME"
}

# `$SSH_PUBLIC_KEY` is a runtime variable because `/home/$USERNAME/.ssh` is normally
# a mounted volume, so a key baked in at build time would be shadowed by the mount
# and silently ignored
seed_authorized_keys() {
    mkdir -p "$home/.ssh"

    if [ -n "${SSH_PUBLIC_KEY:-}" ]; then
        if ! grep -qxF "$SSH_PUBLIC_KEY" "$home/.ssh/authorized_keys" 2>/dev/null; then
            log "appending \$SSH_PUBLIC_KEY to authorized_keys"
            printf '%s\n' "$SSH_PUBLIC_KEY" >> "$home/.ssh/authorized_keys"
        fi
    fi

    if [ ! -s "$home/.ssh/authorized_keys" ]; then
        log "WARNING: $home/.ssh/authorized_keys is empty, nobody will be able to log in."
        log "WARNING: pass -e SSH_PUBLIC_KEY=\"\$(cat ~/.ssh/ytdlp.pub)\" or put the key in the mounted volume."
    fi
}

# a host key baked into the image would be identical for everyone who pulls the image,
# (and would in any case be hidden by the volume mounted over `$keys`)
ensure_host_keys() {
    mkdir -p "$keys"

    if [ ! -f "$keys/ssh_host_ed25519_key" ]; then
        log "generating an ed25519 host key"
        ssh-keygen -q -t ed25519 -N '' -f "$keys/ssh_host_ed25519_key"
    fi

    if [ ! -f "$keys/ssh_host_rsa_key" ]; then
        log "generating an RSA host key"
        ssh-keygen -q -t rsa -b 4096 -N '' -f "$keys/ssh_host_rsa_key"
    fi

    # `sshd` ignores a private host key that has any group or other permission bit set:
    # `sshkey_perm_ok()` rejects `st.st_mode & 077`, but only when the file is owned by
    # the user running `sshd` (which it is, after `fix_ownership`)
    #
    # the keys generated above are already `0600`, but the ones restored from a backup
    # or left over from an older revision of this image can have looser modes, and those
    # are skipped by the checks above, so the mode is enforced on every start
    for hostKey in "$keys"/ssh_host_*_key; do
        [ -f "$hostKey" ] || continue

        if ! chmod 0600 "$hostKey" 2>/dev/null; then
            log "WARNING: could not chmod 0600 $hostKey"
        fi

        hostKeyMode="$(stat -c '%a' "$hostKey")"
        case "$hostKeyMode" in
            400 | 600) ;;
            *)
                log "WARNING: $hostKey is mode $hostKeyMode, so sshd will ignore it"
                log "WARNING: this volume does not seem to honour POSIX permissions (bind mounts from macOS/Windows hosts do not), so try a proper Docker volume instead"
                ;;
        esac
    done
}

render_sshd_config() {
    log "rendering $sshd_config for port $SSH_PORT"
    envsubst '$SSH_PORT $NON_ROOT_SSH_FOLDER' \
        < /usr/local/share/yt-dlp/sshd_config.template \
        > "$sshd_config"
}

# paths that the container can live without owning, a read-only bind mount
# over a config file is a reasonable thing to do, and it should not stop
# the container from starting
chown_optional() {
    for path in "$@"; do
        [ -e "$path" ] || continue
        if ! chown -R "$PUID:$PGID" "$path" 2>/dev/null; then
            log "WARNING: could not chown $path (read-only mount?), continuing"
        fi
    done
}

fix_ownership() {
    # not recursive, `$home` contains the downloads mount (which can be big)
    if ! chown "$PUID:$PGID" "$home" 2>/dev/null; then
        log "WARNING: could not chown $home (read-only mount?), continuing"
    fi

    chown_optional "$home/.config" "$home/.cache" "$home/ydl.sh"

    # `sshd` will not start without these
    chown -R "$PUID:$PGID" "$home/.ssh" "$NON_ROOT_SSH_FOLDER"

    chmod 0700 "$home/.ssh"
    if [ -f "$home/.ssh/authorized_keys" ]; then
        chmod 0600 "$home/.ssh/authorized_keys"
    fi

    if [ -d "$home/downloads" ] \
        && [ "$(stat -c '%u:%g' "$home/downloads")" != "$PUID:$PGID" ]; then
        log "fixing ownership of $home/downloads (mount point only)"
        chown "$PUID:$PGID" "$home/downloads"
    fi
}

start_cron() {
    if [ "${ENABLE_CRON:-1}" != "1" ]; then
        log "cron jobs disabled (ENABLE_CRON=${ENABLE_CRON:-1})"
        return 0
    fi

    log "starting supercronic"
    if is_root; then
        su-exec "$USERNAME" supercronic /etc/supercronic/crontab &
    else
        supercronic /etc/supercronic/crontab &
    fi
    # su-exec exec's into supercronic, so this is supercronic's own PID
    echo "$!" > "${CRON_PIDFILE:-/tmp/supercronic.pid}"
}

if [ "$#" -eq 0 ]; then
    set -- /usr/sbin/sshd -f "$sshd_config" -D -e
fi

if is_root; then
    remap_account
    seed_authorized_keys
    ensure_host_keys
    render_sshd_config
    fix_ownership
    start_cron

    log "dropping privileges to $USERNAME ($PUID:$PGID) and running: $*"
    exec su-exec "$USERNAME" "$@"
else
    log "not running as root ($(id -u):$(id -g)), skipping account and ownership setup"
    seed_authorized_keys
    ensure_host_keys
    render_sshd_config
    start_cron

    log "running: $*"
    exec "$@"
fi
