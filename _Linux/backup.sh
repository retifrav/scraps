#!/bin/bash

workingdir="/root/backups/YOUR-WEBSITE_backup_$(date +%Y.%m.%d-%H%M%S)"
mkdir $workingdir

tar -czvf $workingdir/www.tar.gz /var/www/

mysqldump -v -uYOUR-MYSQL-USER YOUR-DATABASE-NAME --routines -r $workingdir/YOUR-DATABASE-NAME.sql
tar -czf $workingdir/YOUR-DATABASE-NAME.tar.gz -C $workingdir/ .
rm $workingdir/YOUR-DATABASE-NAME.sql

tar -czf $workingdir/USER-FOR-TELEGRAM-BOT.tar.gz /home/USER-FOR-TELEGRAM-BOT/
tar -czf $workingdir/TELEGRAM-BOT.tar.gz /usr/local/bin/TELEGRAM-BOT/

tar -czf $workingdir/apache.tar.gz /etc/apache2/apache2.conf /etc/apache2/ports.conf

tar -czf $workingdir/ngingx.tar.gz /etc/nginx/nginx.conf /etc/nginx/sites-available/ /etc/nginx/sites-enabled/

cp /etc/mysql/my.cnf $workingdir
cp /etc/systemd/system/telegram-bot-TELEGRAM-BOT.service $workingdir

cp /root/backups/backup.sh $workingdir/
cp /root/.bashrc $workingdir/
cp /root/.my.cnf $workingdir/

cp /var/spool/cron/crontabs/root $workingdir/

cp /root/backups/backup.sh $workingdir/
