## New GNU/Linux server

<!-- MarkdownTOC -->

- [Distribution](#distribution)
- [Non-root user](#non-root-user)
- [SSH](#ssh)
- [Packages](#packages)
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

### Non-root user

If your default user is root, create a non-root user and add it to `sudo` group:

``` sh
$ adduser ubuntu
$ usermod -aG sudo ubuntu
```

Make an SSH key for it and put its public key into `/home/ubuntu/.ssh/authorized_keys`.

### SSH

Disable SSH passwords and root login (*after [generating](https://github.com/retifrav/scraps/blob/master/_linux/ssh.md#generate-a-new-ssh-key) and deploying your public SSH key*):

``` sh
$ sudo nano /etc/ssh/sshd_config
```
```
PubkeyAuthentication yes
PasswordAuthentication no
PermitEmptyPasswords no
PermitRootLogin no
```
``` sh
$ sudo systemctl restart sshd.service
```

### Packages

[Fuck Snap](./snap-ram.png), as usual:

``` sh
sudo systemctl stop snapd \
&& sudo systemctl disable snapd \
&& sudo apt autoremove --purge snapd gnome-software-plugin-snap \
&& sudo systemctl daemon-reload \
&& rm -rf ~/snap \
&& sudo rm -rf /var/snap /var/cache/snapd /usr/lib/snapd
```

Update the packages:

``` sh
$ sudo apt update
$ sudo apt upgrade
```

Install some stuff:

``` sh
$ sudo apt install ca-certificates gpg wget mc
```

Improve Python setup:

``` sh
$ sudo apt install python3-pip
$ sudo ln -s /usr/bin/python3 /usr/local/bin/python
```

Install and configure [fail2ban](https://github.com/retifrav/scraps/blob/master/_linux/index.md#fail2ban).

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
