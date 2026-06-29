#!/bin/sh

# comment/delete to disable cron jobs
supercronic /crontab &

exec /usr/sbin/sshd -f ${NON_ROOT_SSH_FOLDER}/sshd_config -D
