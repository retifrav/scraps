#!/bin/bash

telescripts_path=/home/showman/telescripts

info_web="*Web-servers*
----------------------
$(systemctl status nginx.service | sed -r 's/[_]+/-/g' )

$(systemctl status kestrel-protvshows.service | sed -r 's/[_]+/-/g')

"

info_mysql="*MySQL*
----------------------
$(mysqladmin ping)
$(mysqladmin status)

"

info_cpu="*CPU*
----------------------
used: $(top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{print 100 - $1"%% of 100%%"}')

"

info_ram="*RAM*
----------------------
free: $(free -m | grep Mem | awk '{print $4}') MB of $(free -m | grep Mem | awk '{print $2}') MB total

"

info_space="*HDD*
----------------------
$(df -h --output=source,size,pcent | sed -r 's/[%]+/%%/g')
"

text=$(printf "$info_web$info_mysql$info_cpu$info_ram$info_space")
printf '%s\n' "$text" > ${telescripts_path}/status.txt
