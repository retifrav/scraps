## TeamCity

<!-- MarkdownTOC -->

- [Backup](#backup)
    - [Querying API with 2FA enabled](#querying-api-with-2fa-enabled)
    - [Restoring from backup](#restoring-from-backup)
- [Upgrading](#upgrading)
    - [Main service](#main-service)
    - [Build-agent](#build-agent)

<!-- /MarkdownTOC -->

### Backup

``` sh
$ dt="$(date +%Y.%m.%d-%H%M%S)"
$ backupname="teamcity_backup_$dt"
$ workingdir="/path/to/backups/$backupname"
$ mkdir -p $workingdir

# wait some time after executing this, the archive needs to be made
$ teamcityBackup="$(curl --silent --basic --user "YOUR-TEAMCITY-USERNAME:YOUR-TEAMCITY-PASSWORD" -X POST "http://localhost/teamcity/httpAuth/app/rest/server/backup?includeDatabase=true&includeConfigs=true&includeSupplementaryData=true&includeBuildLogs=true&fileName=teamcity-backup")"

$ mv /home/teamcity/.BuildServer/backup/$teamcityBackup $workingdir

# just in case also make a manual backup
$ sudo -u postgres pg_dump teamcity > $workingdir/teamcity.sql

$ tar -czf $workingdir/teamcity.tar.gz \
    /home/teamcity/TeamCity/conf/server.xml \
    /home/teamcity/TeamCity/buildAgent/conf/buildAgent.properties \
    /home/teamcity/.BuildServer/config/main-config.xml \
    /etc/systemd/system/teamcity-buildagent.service \
    /etc/systemd/system/teamcity.service \
    $workingdir/teamcity.sql \
    $workingdir/$teamcityBackup

$ rm $workingdir/teamcity.sql
$ rm $workingdir/$teamcityBackup
```

#### Querying API with 2FA enabled

If you have 2FA enabled, then your backup request will fail. Create an access token and execute this request instead:

``` sh
$ teamcityBackup="$(curl --silent -H "Authorization: Bearer YOUR-TEAMCITY-ACCESS-TOKEN" -X POST "http://localhost/teamcity/app/rest/server/backup?includeDatabase=true&includeConfigs=true&includeSupplementaryData=true&includeBuildLogs=true&fileName=teamcity-backup")"
```

Note that there is no `httpAuth` in the URL.

Resulting backup archive will be at `/home/teamcity/.BuildServer/backup/$teamcityBackup` (*give it some time to make*).

#### Restoring from backup

Out of all that the `teamcity-backup_DATE_TIME.zip` seems to be the only thing that is needed when restoring from a backup on a fresh installation of TeamCity, as it includes the database too, so actually that manual step with `pg_dump` is redundant.

The PostgreSQL database needs to already exist and be empty, so:

``` sh
$ psql -U teamcity -d postgres
postgres=# CREATE DATABASE teamcity;
$ \q
```

And then you just start a new "clean" TeamCity instance on some new server (*if it is a Docker container, have database `/var/lib/postgresql/data/` path mapped for persistency*), select restoring option from the web interface and point it to that ZIP archive (*might need to upload it to the target host/container local/mapped filesystem beforehand, is it might be too big for uploading via web-browser*).

### Upgrading

#### Main service

Download new version archive (*for example `TeamCity-2023.05.3.tar.gz`*). Unpack it somewhere on your server.

Stop the services:

``` sh
$ sudo systemctl stop teamcity-buildagent.service
$ sudo systemctl stop teamcity.service
```

Make a [backup](#backup) of current installation.

Replace existing installation in `/home/teamcity/TeamCity` with this new unpacked version:

``` sh
$ mv /home/teamcity/{TeamCity,TeamCity-backup}
$ mv /path/to/new/TeamCity /home/teamcity/TeamCity
```

Restore the configs:

``` sh
$ cp /home/teamcity/{TeamCity-backup,TeamCity}/conf/server.xml
$ cp /home/teamcity/{TeamCity-backup,TeamCity}/buildAgent/conf/buildAgent.properties
```

Start services:

``` sh
$ sudo systemctl start teamcity.service
$ sudo systemctl start teamcity-buildagent.service
```

Open your TeamCity URL. It will likely show a [maintenance page](https://jetbrains.com/help/teamcity/teamcity-maintenance-mode.html), saying that data format needs go be updated or something. If that page will be failing with loading resources (*CSS, JS and many other files with 404 error*), then probably something got messed up with the base URL or/and your reverse-proxy. Most likely the new `/home/teamcity/TeamCity/webapps/ROOT` needs to be renamed to `/home/teamcity/TeamCity/webapps/teamcity` again (*also check what you have in your `/home/teamcity/.BuildServer/config/main-config.xml`*).

If not, then until you figure out what's wrong, you can try to make an SSH tunnel to TeamCity's port (`8003`) directly from your machine:

``` sh
$ ssh -N -L 8080:localhost:8003 your-server
```

and then open <http://localhost:8080>.

Either way, when the page will load correctly (*the web-form is composed with JS scripts, which have to load successfully, which is why it was failing before*), you will be able to enter the admin token (*from `/home/teamcity/TeamCity/logs/teamcity-server.log`*) and perform the required maintenance/upgrade actions.

#### Build-agent

Build-agent is included in the main service archive (*that same `TeamCity-2023.05.3.tar.gz` or whichever you got*), so it should be all good (*don't forget to restore its config in `/home/teamcity/TeamCity/buildAgent/conf/buildAgent.properties`*).

If build-agent page says "*Agent has unregistered (will upgrade)*", check its logs in `/home/teamcity/TeamCity/buildAgent/logs/upgrade.log`. If it says something like "*Failed to start upgrade process. Return code was 143 is not equal to zero*", add that code to its systemd service:

``` sh
$ sudo nano /etc/systemd/system/teamcity-buildagent.service
```
``` ini
[Service]
Type=forking
WorkingDirectory=/home/teamcity/TeamCity/buildAgent
ExecStart=/home/teamcity/TeamCity/buildAgent/bin/agent.sh start
ExecStop=/home/teamcity/TeamCity/buildAgent/bin/agent.sh stop
Restart=always
RestartSec=10
SyslogIdentifier=TeamCity-BuilAgent
User=teamcity
Group=teamcity

# agent will exit with 143 during upgrade process
SuccessExitStatus=143 0
```

Restart the agent service and watch that `upgrade.log` until it stops getting new records:

``` sh
$ sudo -u teamcity tail -f /home/teamcity/TeamCity/buildAgent/logs/upgrade.log
```

If it does the same operations over and over in an endless loop, stop the service, stop the agent manually (*just in case*) and start it manually:

``` sh
$ sudo systemctl stop teamcity-buildagent.service

$ sudo -u teamcity /home/teamcity/TeamCity/buildAgent/bin/agent.sh stop
Stopping TeamCity build agent...
Java executable is found: '/usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java'
Starting TeamCity Build Agent Launcher...
Agent home directory is /home/teamcity/TeamCity/buildAgent
Unable to locate agent port file: /home/teamcity/TeamCity/buildAgent/logs/buildAgent.xmlRpcPort
Unable to locate agent port file: /home/teamcity/TeamCity/buildAgent/logs/buildAgent.xmlRpcPort
Agent is not running?
Agent is not running?
Failed to shutdown agent gracefully: Connection refused (Connection refused)
Failed to shutdown agent gracefully: Connection refused (Connection refused)
Cannot stop agent gracefully, you can try to kill agent by '/home/teamcity/TeamCity/buildAgent/bin/agent.sh stop kill' command

$ sudo -u teamcity /home/teamcity/TeamCity/buildAgent/bin/agent.sh stop kill
Stopping TeamCity build agent...
../logs/buildAgent.pid not found, nothing to stop.

$ sudo -u teamcity /home/teamcity/TeamCity/buildAgent/bin/agent.sh start
```

Then it should do everything correctly, and its page in TeamCity will show it being connected and ready. Stop it manually and start it back as a service:

``` sh
$ sudo -u teamcity /home/teamcity/TeamCity/buildAgent/bin/agent.sh stop
Stopping TeamCity build agent...
Java executable is found: '/usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java'
Starting TeamCity Build Agent Launcher...
Agent home directory is /home/teamcity/TeamCity/buildAgent

$ sudo systemctl start teamcity-buildagent.service
```
