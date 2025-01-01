# teamcity-agent

This is a Docker image for [TeamCity agent](https://jetbrains.com/help/teamcity/build-agent.html).

It is 1-to-1 the same as the [official image](https://hub.docker.com/r/jetbrains/teamcity-agent), the only difference is additional commands to "unhardcode" the `UID`/`GID` values, so the image could be used in [Container Manager](https://synology.com/en-global/dsm/feature/docker) on Synology DSM, as described in [this bugreport](https://youtrack.jetbrains.com/issue/BND-1756/Cannot-use-Docker-Image-on-Synology-NAS-as-UID-GID-is-static#focus=Comments-27-6921347.0-0).

Pre-built image is published on [Docker Hub](https://hub.docker.com/r/decovar/teamcity-agent).

## Building

``` sh
$ export IMAGE_NAME='teamcity-agent'
$ export VER='2024.12'

$ docker build . -f ./Dockerfile \
    --build-arg VER=$VER \
    --platform linux/amd64 \
    --tag $IMAGE_NAME:$VER \
    --tag $IMAGE_NAME:latest

$ docker images
REPOSITORY                  TAG                       IMAGE ID       CREATED          SIZE
teamcity-agent              2024.12                   1fe7beee6dc9   13 seconds ago   2.08GB
teamcity-agent              latest                    1fe7beee6dc9   13 seconds ago   2.08GB
```

## Self-signed SSL/TLS on server

<https://github.com/JetBrains/teamcity-docker-agent/issues/41>

If you get errors on agent trying to talk to TeamCity server like `javax.net.ssl.SSLHandshakeException`, then get your server `cert.pem` and create the thing for the agent (*that command works on Mac OS, at least*):

``` sh
$ keytool -importcert -alias SOME \
    -file /path/to/cert.pem \
    -deststoretype jks \
    -keystore ./keystore.jks \
    -storepass SOME_PASSWORD
```

Then place the resulting `keystore.jks` into `/data/teamcity_agent/conf/trustedCertificates/` (*which, in case of running the agent as a Docker container, will/should be in a mapped volume*) and add `TEAMCITY_AGENT_OPTS` launch arguments via `environment`:

``` yaml
agent:
    image: teamcity-agent
    container_name: teamcity-agent
    environment:
      SERVER_URL: https://192.168.1.111:12345
      TEAMCITY_AGENT_OPTS: -Djavax.net.ssl.keyStore=/data/teamcity_agent/conf/trustedCertificates/keystore.jks -Djavax.net.ssl.trustStore=/data/teamcity_agent/conf/trustedCertificates/keystore.jks -Djavax.net.ssl.keyStorePassword=SOME-PASSWORD -Djavax.net.ssl.trustStorePassword=SOME-PASSWORD
    restart:  unless-stopped
    volumes:
      - /volume1/docker/teamcity/data/agent/conf:/data/teamcity_agent/conf
      - /volume1/docker/teamcity/data/agent/work:/opt/buildagent/work
      - /volume1/docker/teamcity/data/agent/logs:/opt/buildagent/logs
      - /volume1/docker/teamcity/data/agent/system:/opt/buildagent/system
```

The last two (*`-Djavax.net.ssl.trustStore` and `-Djavax.net.ssl.trustStorePassword`*) don't seem to be needed.
