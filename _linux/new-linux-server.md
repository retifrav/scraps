## New GNU/Linux server

<!-- MarkdownTOC -->

- [Distribution](#distribution)
- [SSH](#ssh)
- [Packages](#packages)

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
$ sudo systemctl stop snapd
$ sudo systemctl disable snapd
$ sudo apt autoremove --purge snapd gnome-software-plugin-snap
$ sudo systemctl daemon-reload
$ rm -rf ~/snap
$ sudo rm -rf /var/snap /var/cache/snapd /usr/lib/snapd
```
