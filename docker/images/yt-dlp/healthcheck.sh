#!/bin/sh

# `sshd` exiting takes the container down on its own, because it is what
# the `ENTRYPOINT` script exec's into. This check exists mainly to also notice
# `supercronic` dying, which would otherwise leave the container looking healthy
# while no cron job ever runs again (that is if it was meant to be enabled at all)

set -eu

sshd_pidfile="$NON_ROOT_SSH_FOLDER/sshd.pid"
cron_pidfile="${CRON_PIDFILE:-/tmp/supercronic.pid}"

if [ ! -f "$sshd_pidfile" ]; then
    echo "sshd pid file $sshd_pidfile is missing"
    exit 1
fi

if ! kill -0 "$(cat "$sshd_pidfile")" 2>/dev/null; then
    echo "sshd is not running"
    exit 1
fi

if [ "${ENABLE_CRON:-1}" = "1" ]; then
    if [ ! -f "$cron_pidfile" ] || ! kill -0 "$(cat "$cron_pidfile")" 2>/dev/null; then
        echo "supercronic is not running"
        exit 1
    fi
fi

echo "ok"
