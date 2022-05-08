## TeamCity

<!-- MarkdownTOC -->

- [Updating](#updating)

<!-- /MarkdownTOC -->

### Updating

1. Stop the services for both build agent and TeamCity itself;
2. Make a backup of the current installation and its database:
    + `/home/teamcity/TeamCity`;
    + `/home/teamcity/.BuildServer`;
3. Download the latest version archive;
4. Make a copy of `/home/teamcity/TeamCity/conf/server.xml` and `/home/teamcity/TCBckp/buildAgent/conf/buildAgent.properties`;
5. Delete current installation (`rm -r /home/teamcity/TeamCity/*`). The data is stored in `/home/teamcity/.BuildServer`, so it's okay;
6. Unpack downloaded archive with the latest version to `/home/teamcity/TeamCity/`;
7. Replace `/home/teamcity/TeamCity/conf/server.xml` and `/home/teamcity/TCBckp/buildAgent/conf/buildAgent.properties` with the copies you made;
8. Rename `/home/teamcity/TeamCity/webapps/ROOT` to `/home/teamcity/TeamCity/webapps/teamcity` (or whichever is set for `rootURL` in `/home/teamcity/.BuildServer/config/main-config.xml`);
9. Start the TeamCity service;
10. Open the page, it will request to perform some maintenance, give it access token from `/home/teamcity/TeamCity/logs/teamcity-server.log`.
