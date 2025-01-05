ARG VER="2.0.6"

FROM timshel/sharebin:$VER

USER root

ARG UID=1034
ARG USERNAME=docker

ARG GID=65537
ARG GROUPNAME=dockers

RUN echo "Unhardcoding UID to $UID and GID to $GID" \
    && groupadd --gid $GID $GROUPNAME \
    && adduser --uid $UID --gid $GID --system $USERNAME \
    && chown -R $USERNAME:$GROUPNAME /app

USER $USERNAME
