## New GNU/Linux server

<!-- MarkdownTOC -->

- [Distribution](#distribution)
- [SSH](#ssh)
- [Packages](#packages)
    - [Python](#python)
    - [NGINX](#nginx)

<!-- /MarkdownTOC -->

### Distribution

This will be about Ubuntu:

``` sh
$ lsb_release -a
Description:    Ubuntu 22.04.3 LTS
Release:    22.04
Codename:    jammy
```

### SSH

Disable SSH passwords (*but first [generate](https://github.com/retifrav/scraps/blob/master/_linux/ssh.md#generate-a-new-ssh-key) and deploy your public SSH key*):

``` sh
$ sudo nano /etc/ssh/sshd_config
```
```
ChallengeResponseAuthentication no
PasswordAuthentication no
PermitEmptyPasswords no
```
``` sh
$ sudo systemctl restart sshd.service
```

### Packages

Update the system:

``` sh
$ sudo apt update
$ sudo apt upgrade
```

Fuck Snap, as usual:

``` sh
sudo systemctl stop snapd \
&& sudo systemctl disable snapd \
&& sudo apt autoremove --purge snapd gnome-software-plugin-snap \
&& sudo systemctl daemon-reload \
&& rm -rf ~/snap \
&& sudo rm -rf /var/snap /var/cache/snapd /usr/lib/snapd
```

Install some stuff:

```
$ sudo apt install ca-certificates gpg wget mc
```

#### Python

```
$ sudo apt install python3-pip
$ sudo ln -s /usr/bin/python3 /usr/local/bin/python
```

#### NGINX

<http://nginx.org/en/linux_packages.html#Ubuntu>

Add a package source and install:

``` sh
$ curl https://nginx.org/keys/nginx_signing.key | gpg --dearmor \
    | sudo tee /usr/share/keyrings/nginx-archive-keyring.gpg >/dev/null
$ gpg --dry-run --quiet --no-keyring --import --import-options import-show /usr/share/keyrings/nginx-archive-keyring.gpg
$ echo "deb [signed-by=/usr/share/keyrings/nginx-archive-keyring.gpg] \
http://nginx.org/packages/ubuntu `lsb_release -cs` nginx" \
    | sudo tee /etc/apt/sources.list.d/nginx.list

$ sudo apt update
$ sudo apt install nginx

$ nginx -version
```

Add a basic website to check that it works:

``` sh
$ sudo mkdir -p /var/www/some && cd $_ && cd ..
$ sudo git clone https://github.com/retifrav/default-web-server-index.git ./some
$ sudo chown -R www-data:www-data /var/www
```

Enable it in the config:

``` sh
$ sudo nano /etc/nginx/conf.d/default.conf
```
``` nginx
server {
    listen       80;
    server_name  localhost;
    location / {
        root  /var/www/some;
        index index.html;
    }

    error_page 404 /404.html;
    error_page 403 /403.html;
    error_page 401 /401.html;
}
```
``` sh
$ sudo systemctl start nginx.service
$ sudo systemctl status nginx.service
```

Open <http://SERVER-IP-ADDRESS/> (*or <http://SERVER-IP-ADDRESS:80/>*).
