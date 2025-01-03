ARG VER="2.0.5"

FROM timshel/sharebin:$VER

USER root

ENV UID=1034
ENV USERNAME=docker

ENV GID=65537
ENV GROUPNAME=dockers

RUN echo "Unhardcoding UID to $UID and GID to $GID" \
    && groupadd --gid $GID $GROUPNAME \
    && adduser --uid $UID --gid $GID --system $USERNAME \
    && chown -R $USERNAME:$GROUPNAME /app

USER $USERNAME
