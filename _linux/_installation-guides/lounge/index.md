## The Lounge

Self-hosted web IRC client and bouncer: <https://thelounge.chat/>

<!-- MarkdownTOC -->

- [Docker container](#docker-container)
- [NGINX reverse-proxy](#nginx-reverse-proxy)
- [CSS and themes](#css-and-themes)
- [SASL](#sasl)
- [identd](#identd)

<!-- /MarkdownTOC -->

First of all, the usual new GNU/Linux server [routine](/_linux/new-linux-server.md). Then [install Docker](/docker/index.md#linux) and put it into rootless mode.

## Docker container

``` sh
$ sudo mkdir -p /data/lounge
$ sudo cp ~/downloads/hc.sqlite /data/lounge/
$ sudo chown -R 100998:100998 /data/lounge
$ sudo chmod -R 755 /data/lounge

$ docker run -d \
    --name lounge \
    --restart=unless-stopped \
    -e TZ=Europe/Amsterdam \
    -p 127.0.0.1:9000:9000 \
    -v /data/lounge:/var/opt/thelounge \
    ghcr.io/thelounge/thelounge:latest

$ docker exec -it --user 1000 lounge thelounge help
$ docker exec -it --user 1000 lounge thelounge add SOME-USERNAME

$ sudo micro /data/lounge/config.js
```
``` js
// ...

public: false,

// ...

reverseProxy: true,

// ...

https: {
    enable: false,
    key: "",
    certificate: "",
    ca: "",
},

// ...

disableMediaPreview: true

// ...

fileUpload: {
    enable: false,
    maxFileSize: 10240,
    baseUrl: null,
},

// ...

leaveMessage: "ololo",

// ...
```
``` sh
$ docker stop lounge
$ docker start lounge
```

## NGINX reverse-proxy

``` sh
$ sudo mkdir /var/log/nginx/lounge.YOUR.HOST
$ sudo chown -R www-data:www-data /var/log/nginx/lounge.YOUR.HOST

$ sudo micro /etc/nginx/conf.d/lounge.YOUR.HOST
```
``` nginx
map $http_upgrade $connection_upgrade {
    default upgrade;
    '' close;
}

server {
    listen 443 ssl;
    listen [::]:443 ssl;

    server_name lounge.YOUR.HOST;

    ssl_certificate /path/to/your/fullchain.pem;
    ssl_certificate_key /path/to/your/key.pem;

    access_log /var/log/nginx/lounge.YOUR.HOST/access.log main;
    error_log /var/log/nginx/lounge.YOUR.HOST/error.log notice;

    location / {
        proxy_pass http://127.0.0.1:9000;

        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection $connection_upgrade;
        proxy_set_header X-Forwarded-For $remote_addr;
        proxy_set_header X-Forwarded-Proto $scheme;
        # default timeout is 1 minute
        proxy_read_timeout 1d;
    }
}

server {
    listen 80;
    listen [::]:80;
    server_name lounge.YOUR.HOST;
    return 301 https://$server_name$request_uri;
}
```

## CSS and themes

Themes are supposed to be installed as npm packages, but for the love of god, do check the package contents and its dependencies before the installation, and maybe even resort to manual copying of the themes files. For the time being the [Dracula theme](https://draculatheme.com/thelounge) (*version [1.0.6](https://github.com/dracula/thelounge/commit/a94fcaff6f16d7a6808c520a605c4bb0801a5438)*) is quite a minimal package with no dependencies (*and a better chat layout too*):

``` sh
$ docker exec -it --user 1000 lounge thelounge install thelounge-theme-dracula-official
```

Still, you might want to further customize the CSS, things like:

``` css
#sidebar {
    width: 220px;
}

.messages .msg {
    font-family: "Courier New", monospace;
    font-size: 18px !important;
}

.msg .user {
    font-weight: bold;
}

table.channel-list td.channel {
    min-width: 180px;
}
#chat table.channel-list .topic {
    white-space: initial;
}

#chat .time {
    color: #777;
}
```

## SASL

If you'd like to connect/authenticate with a SASL certificate, then set `Client certificate (SASL EXTERNAL)` in the network authentication settings (*in the Lounge web GUI*) and save the settings. After that Lounge should generate a certificate at `/data/lounge/certificates/` with the name matching this network `uuid` from your `/data/lounge/users/SOME-USERNAME.json`. Get the fingerprint of that certificate:

``` sh
$ sudo openssl x509 -in /data/lounge/certificates/NETWORK-UUID.crt -noout -sha512 -fingerprint
sha512 Fingerprint=F1:NG:3R:PR:1N:T...
```

And add it to your IRC account via `NickServ`:

```
/msg NickServ CERT ADD F1:NG:3R:PR:1N:T...
```

Of course, now you will need to authenticate with it first, since you connected with unknown certificate. Or maybe switch back to `Username + password (SASL PLAIN)`, add certificate fingerprint via `NickServ` and switch to `Client certificate (SASL EXTERNAL)` again.

## identd

If you are going to use [identd](https://thelounge.chat/docs/guides/identd-and-oidentd#built-in-identd-server), then enable it in the `/data/lounge/config.js`:

``` js
// ...

identd: {
    enable: true,
    port: 9001,
},

// ...
```

and then add `-p 113:9001` to `docker run`. But if your Docker is rootless, it won't allow to bind to `113` port, so that needs to be allowed for your current non-root user, probably like this (*haven't actually tested*):

``` sh
$ systemctl --user stop docker.service

$ which rootlesskit
/usr/bin/rootlesskit
$ sudo setcap cap_net_bind_service=ep $(which rootlesskit)
$ getcap $(which rootlesskit)

$ systemctl --user start docker.service
```

You will also likely need to open that port in your firewall/VPS panel.
