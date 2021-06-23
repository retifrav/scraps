#!/bin/bash

uploadToCloud=0

# command line options for the script
while getopts "c" opt; do
  case $opt in
    c)
        uploadToCloud=1
        ;;
    \?)
        echo "Unknown option -$OPTARG" >&2
        exit 1
        ;;
  esac
done

archName="backup_$(date +%Y.%m.%d-%H%M%S)"
workingdir="/root/backups/$archName"
mkdir $workingdir

mysqldump database-name --routines --no-tablespaces -r $workingdir/database.sql

tar -czf $workingdir/showman.tar.gz /home/showman/
tar -czf $workingdir/telegrambot.tar.gz /usr/local/bin/showman/

tar -czf $workingdir/services.tar.gz /etc/systemd/system/some.service /etc/systemd/system/another.service

tar -czf $workingdir/www.tar.gz /var/www/website/

tar -czf $workingdir/nginx.tar.gz /etc/nginx/nginx.conf /etc/nginx/sites-available/ /etc/nginx/sites-enabled/

cp /root/.bashrc $workingdir/
cp /root/.my.cnf $workingdir/

# scripts
tar -czf $workingdir/scripts.tar.gz /root/*.sh /root/backups/*.sh

# GoAccess
tar -czf $workingdir/goaccess.tar.gz /etc/goaccess/goaccess.conf

# Jupyter
tar -czf $workingdir/jupyter.tar.gz /home/jupyter/.jupyter/jupyter_notebook_config.py /var/www/jupyter

# cron
cp /var/spool/cron/crontabs/root $workingdir/

# sudoerrs
tar -czf $workingdir/sudoers.tar.gz --exclude=README /etc/sudoers.d/*

tarFile="/data/backups/$archName.tar"
tar -C $workingdir -cf $tarFile .
chown -R teamcity:teamcity /data/backups

if [ $uploadToCloud -eq 1 ]; then
    echo "Uploading the backup to cloud"
    echo "Encrypting it first..."
    gpg --encrypt --recipient your@email.com $tarFile
    echo "Now uploading..."
    rclone copy "$tarFile.gpg" s3:/bucket-name/archive/ -P
    rclone copyto "$tarFile.gpg" s3:/bucket-name/latest.gpg -P && rm "$tarFile.gpg"
else
    echo "Won't upload the backup to cloud"
fi
