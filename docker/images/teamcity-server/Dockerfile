ARG VER="2024.12"

FROM jetbrains/teamcity-server:$VER

# these should already be defined in the base image
#ENV TEAMCITY_DATA_PATH=/data/teamcity_server/datadir \
#    TEAMCITY_DIST=/opt/teamcity \
#    TEAMCITY_ENV=container

USER root

ENV USERGROUPNAME=tcuser
ENV UID=1034
ENV GID=65537

# original user/group in the base image is tcuser/tcuser:
# https://github.com/JetBrains/teamcity-docker-images/blob/b249d32934e17eaca393fb2bf5f7d20fc5673999/context/generated/linux/Server/Ubuntu/22.04/Dockerfile#L92-L93
#
# or maybe create a new user/group instead of groupmod/usermod:
# && groupadd --gid $GID dockers \
# && adduser --uid $UID --gid $GID --system docker \
RUN echo "Unhardcoding UID to $UID and GID to $GID" \
    && groupmod --gid $GID $USERGROUPNAME \
    && usermod --non-unique --uid $UID --gid $GID $USERGROUPNAME \
    && chown -R $USERGROUPNAME:$USERGROUPNAME \
        /services \
        $TEAMCITY_DATA_PATH \
        $TEAMCITY_DIST

USER $USERGROUPNAME
