ARG VER="2024.12"

FROM jetbrains/teamcity-agent:$VER

USER root

ENV USERGROUPNAME=buildagent
ENV UID=1034
ENV GID=65537

# original user/group in the base image is buildagent/buildagent:
# https://github.com/JetBrains/teamcity-docker-agent/blob/4d588af9a02f9c1006da98dd321d81baac14e36e/ubuntu/Dockerfile#L68
#
# or maybe create a new user/group instead of groupmod/usermod:
# && groupadd --gid $GID dockers \
# && adduser --uid $UID --gid $GID --system docker \
RUN echo "Unhardcoding UID to $UID and GID to $GID" \
    && groupmod --gid $GID $USERGROUPNAME \
    && usermod --non-unique --uid $UID --gid $GID $USERGROUPNAME \
    && chown -R $USERGROUPNAME:$USERGROUPNAME \
        /services \
        /opt/buildagent

USER $USERGROUPNAME
