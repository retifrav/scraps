#FROM alpine:latest

# JavaScript engine is now required, thanks to YouTube
# https://github.com/yt-dlp/yt-dlp/wiki/EJS
FROM denoland/deno:alpine

RUN deno --version

ARG YTDLP_VERSION_VALUE="2024.12.23"
# for nightly builds it would be `yt-dlp-nightly-builds`
ARG YTDLP_DOWNLOAD_TYPE="yt-dlp"
# for not Alpine-based images it would be `yt-dlp_linux`
ARG YTDLP_DOWNLOAD_NAME="yt-dlp_musllinux"

ARG UID=1034
ARG USERNAME=docker

ARG GID=65537
ARG GROUPNAME=dockers

ARG SSH_PUBLIC_KEY
ARG SSH_PORT=2222

# this should be ARG, but for ENTRYPOINT script it has(?) to be ENV (and isn't meant to be overridden)
ENV NON_ROOT_SSH_FOLDER="/home/$USERNAME/non-root-ssh"

RUN echo "Creating the image user/group with UID $UID and GID $GID" \
    && addgroup \
        --gid $GID \
        $GROUPNAME \
    && adduser \
        --disabled-password \
        --gecos "" \
        --ingroup $GROUPNAME \
        --uid $UID \
        $USERNAME

RUN cd /tmp

RUN wget https://github.com/yt-dlp/$YTDLP_DOWNLOAD_TYPE/releases/download/$YTDLP_VERSION_VALUE/$YTDLP_DOWNLOAD_NAME
RUN chmod +x ./$YTDLP_DOWNLOAD_NAME
RUN mv ./$YTDLP_DOWNLOAD_NAME /usr/bin/yt-dlp

RUN apk add --no-cache --update \
    tzdata \
    openssh \
    ffmpeg \
    gettext \
    screen \
    nano
    #
    #openrc \
    #lsof

# Deno image breaks some things, in particular `/usr/lib` is missing
# from `LD_LIBRARY_PATH` (even though they do add `/usr/local/lib` there),
# and without it certain applications (such as FFmpeg) won't run
#ENV LD_LIBRARY_PATH /usr/lib:/usr/local/lib
RUN echo 'export LD_LIBRARY_PATH=/usr/lib:/usr/local/lib' >> /etc/profile

#RUN ssh-keygen -A
#RUN sed -i s/#Port.*/Port\ $SSH_PORT/ \
#    /etc/ssh/sshd_config
##RUN sed -i s/^#HostKey/HostKey/ \
##    /etc/ssh/sshd_config
##RUN sed -i s/^AuthorizedKeysFile.*/AuthorizedKeysFile\ \\/home\\/%u\\/.ssh\\/authorized_keys/ \
##    /etc/ssh/sshd_config
#RUN sed -i s/#PasswordAuthentication.*/PasswordAuthentication\ no/ \
#    /etc/ssh/sshd_config
#RUN sed -i s/#PermitRootLogin.*/PermitRootLogin\ no/ \
#    /etc/ssh/sshd_config

RUN mkdir -p /home/$USERNAME/.ssh
RUN echo "$SSH_PUBLIC_KEY" > /home/$USERNAME/.ssh/authorized_keys
RUN chmod 644 /home/$USERNAME/.ssh/authorized_keys
#
RUN mkdir -p ${NON_ROOT_SSH_FOLDER}/keys
RUN ssh-keygen -f ${NON_ROOT_SSH_FOLDER}/keys/ssh_host_rsa_key -N '' -t rsa
#RUN ssh-keygen -f ${NON_ROOT_SSH_FOLDER}/keys/ssh_host_dsa_key -N '' -t dsa
#
COPY ./sshd_config.template /tmp/
RUN envsubst < /tmp/sshd_config.template > ${NON_ROOT_SSH_FOLDER}/sshd_config
RUN rm /tmp/sshd_config.template
#
EXPOSE $SSH_PORT

RUN mkdir -p \
    /home/$USERNAME/downloads \
    /home/$USERNAME/.config/yt-dlp
COPY ./config /home/$USERNAME/.config/yt-dlp/
COPY ydl.sh /home/$USERNAME/
RUN chown -R $USERNAME:$GROUPNAME /home/$USERNAME

# only by utilizing a script in ENTRYPOINT one can use the ENV variables
# to let user override those values
COPY entrypoint.sh /

USER $USERNAME

# not needed really, but it is useful to explicitly check that yt-dlp can run at all
RUN yt-dlp --version

ENTRYPOINT [ "/bin/sh", "/entrypoint.sh" ]
