#!/bin/bash

currentdir=$PWD
backupname="YOUR-WEBSITE_backup_$(date +%Y.%m.%d-%H%M%S)"
workingdir="/root/backups/$backupname"
mkdir $workingdir

tar -czvf $workingdir/www.tar.gz /var/www/

mysqldump --defaults-file="/root/.my-USERNAME.cnf" -v -uYOUR-MYSQL-USER YOUR-DATABASE-NAME --routines -r $workingdir/YOUR-DATABASE-NAME.sql
cd $workingdir
tar -czf YOUR-DATABASE-NAME.tar.gz YOUR-DATABASE-NAME.sql
#cd $currentdir
#rm $workingdir/YOUR-DATABASE-NAME.sql

tar -czf $workingdir/USER-FOR-TELEGRAM-BOT.tar.gz /home/USER-FOR-TELEGRAM-BOT/
tar -czf $workingdir/TELEGRAM-BOT.tar.gz /usr/local/bin/TELEGRAM-BOT/

# web servers
#tar -czf $workingdir/apache.tar.gz /etc/apache2/apache2.conf /etc/apache2/ports.conf
tar -czf $workingdir/ngingx.tar.gz /etc/nginx/nginx.conf /etc/nginx/sites-available/ /etc/nginx/sites-enabled/ /etc/nginx/mime.types

tar -czf $workingdir/letsencrypt.tar.gz /etc/letsencrypt/

cp /etc/mysql/my.cnf $workingdir
cp /etc/systemd/system/telegram-bot-TELEGRAM-BOT.service $workingdir

cp /root/.bashrc $workingdir/
cp /root/.my.cnf $workingdir/

cp /var/spool/cron/crontabs/root $workingdir/

# scripts
tar -czf $workingdir/scripts.tar.gz /root/*.sh /root/backups/backup.sh

# sudoerrs
tar -czf $workingdir/sudoers.tar.gz --exclude=README /etc/sudoers.d/*

cp /root/backups/backup.sh $workingdir/

# --- upload all that to remote server
rclone copy $workingdir some-remote:/backups/$backupname -P --transfers 2
