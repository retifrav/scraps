## SSH

<!-- MarkdownTOC -->

- [Generate a new SSH key](#generate-a-new-ssh-key)
    - [Generate a public key from private key](#generate-a-public-key-from-private-key)
- [SSH config example](#ssh-config-example)
- [Ignore changed remote host identification](#ignore-changed-remote-host-identification)
- [Disable SSH passwords](#disable-ssh-passwords)
- [Open a tunnel to some port](#open-a-tunnel-to-some-port)
- [Run a remote command](#run-a-remote-command)
- [Read or source remote encrypted PGP file](#read-or-source-remote-encrypted-pgp-file)
- [SFTP](#sftp)
    - [Listing files](#listing-files)
    - [Copying files](#copying-files)
        - [Download](#download)
        - [Upload](#upload)
    - [Guest access](#guest-access)
- [X11 forwarding](#x11-forwarding)
- [Authentication with 2FA](#authentication-with-2fa)
- [Home folder is encrypted](#home-folder-is-encrypted)
- [It's a laptop, and the lid is closed](#its-a-laptop-and-the-lid-is-closed)

<!-- /MarkdownTOC -->

### Generate a new SSH key

New keys should probably be generated with [Ed25519](https://en.wikipedia.org/wiki/EdDSA#Ed25519):

``` sh
$ cd ~/.ssh
$ ssh-keygen -t ed25519 -a 100 -C "name@example.org"
```

But for old systems you might need to use this:

``` sh
$ cd ~/.ssh
$ ssh-keygen -o -t rsa -b 4096 -a 100 -C "name@example.org"
```

Leave empty password (or whatever) and set the file name. Change permissions for the files: `chmod 600 id_rsa_newkey*`.

#### Generate a public key from private key

If you lost the public key somehow, you can generate it (*exactly the same*) again from your private key:

``` sh
$ ssh-keygen -f ~/.ssh/some -y > ~/Desktop/some.pub
```

### SSH config example

An example of a config section (`~/.ssh/config`) for connecting to some remote host:

``` config
Host some-server                # alias for convenience
HostName 216.18.168.16          # actual host address
IdentityFile ~/.ssh/some-server # SSH key file
User yourname                   # username
```

### Ignore changed remote host identification

When you have the same device, and you keep switching Linux installations on it, but DHCP gives it the same IP, so your `~/.ssh/known_hosts` is not happy about it.

``` sh
$ ssh -o UserKnownHostsFile=/dev/null root@SOME-IP-ADDRESS
```

And that way the "updated" key will not get saved.

### Disable SSH passwords

So you could connect only by using an SSH key. Your public key should be placed into `~/.ssh/authorized_keys` on the remote host.

To disable SSH passwords (*and [root login](/_linux/new-linux-server.md#non-root-user)*):

``` sh
$ sudo nano /etc/ssh/sshd_config
```
``` sh
PubkeyAuthentication yes
PasswordAuthentication no
PermitEmptyPasswords no
ChallengeResponseAuthentication no

# only if you have already created a non-root user (which is always recommended)
#PermitRootLogin no
```

If you would like to allow some user to still use the password, then in the end of file:

``` config
Match User vasya
    PasswordAuthentication yes
```

Don't forget to restart the service:

``` sh
$ sudo systemctl restart sshd.service
```

### Open a tunnel to some port

``` sh
$ ssh -N -L 8080:localhost:8080 USERNAME@HOSTNAME
```

And then, for example, all the HTTP requests you send to http://localhost:8080 on your local machine will be actually sent (*tunneled*) to `8080` port of the remote `HOSTNAME`.

### Run a remote command

For example, check the temperature on your Raspberry Pi device (given that you have this host credentials in your `~/.ssh.config`):

``` sh
$ ssh 192.168.1.10 "sudo vcgencmd measure_temp"
```

### Read or source remote encrypted PGP file

On the remote host create a file:

``` sh
$ nano ~/pwds

my_pwd_ololo="nfSas3SF#f54snCs"
my_pwd_some="cm!jj1i495sfdsgs"
```

Encrypt it:

``` sh
$ gpg --recipient SOME-EMAIL-FROM-KEYCHAIN --encrypt pwds
$ rm pwds
```

Now on your local machine, if you want `source` those to environment variables:

``` sh
$ eval $(ssh THAT-REMOTE-HOST "gpg -dq ~/pwds.gpg")
$ echo $my_pwd_ololo
nfSas3SF#f54snCs
```

Or if you just want to output the contents:

``` sh
$ ssh THAT-REMOTE-HOST "gpg -dq ~/pwds.gpg"
my_pwd_ololo="nfSas3SF#f54snCs"
my_pwd_some="cm!jj1i495sfdsgs"
```

Note, that decryption happens on the remote server, so you don't need to import private key on your local machine. If you trust PGP encryption more than your SSH transport, then download the encrypted file and decrypt it locally (*then, of course, you will need to import that private key*).

### SFTP

#### Listing files

As a `username` connecting to `some.host` with SSH/SFTP on `64` port and a password instead of SSH key:

``` sh
$ echo 'ls /some-folder/' | sftp -q -P 64 username@some.host
username@some.host's password:
sftp> ls /some-folder/
/some-folder/some-files
```

or:

``` sh
$ sftp -q -P 64 username@some.host <<< 'ls /some-folder/'
username@some.host's password:
sftp> ls /some-folder/
/some-folder/some-files
```

#### Copying files

##### Download

``` sh
$ sftp USERNAME@some.host:some.txt .
```

That will download `/home/USERNAME/some.txt` file from remote server to local `./some.txt`.

##### Upload

To upload a file to remote server:

``` sh
$ sftp USERNAME@some.host:. <<< $'put ./some.txt'
```

That will upload a local `./some.txt` file to remote server to `/home/USERNAME/some.txt`.

#### Guest access

Based on <https://passingcuriosity.com/2014/openssh-restrict-to-sftp-chroot/>.

When needed to transfer big amounts of data, cloud uploads might not be suitable because of possible disconnections, non-resumable downloads, auto-generated URLs and so on. In that case you can give a person SFTP access to your server.

Add a new group and user:

``` sh
$ sudo addgroup guestsftp
$ sudo adduser --ingroup guestsftp someguest
```

If it won't get password (*for example, if you created this user with `--disabled-password`*), set it:

``` sh
$ sudo passwd someguest
```

But actually it's better to use an SSH key instead, so [generate one](#generate-a-new-ssh-key) and put the public part into `/home/someguest/.ssh/authorized_keys`. Use passwords only if your user/guest is retarded enough to not be able to handle SSH keys.

Create a folder for chroot:

``` sh
$ sudo mkdir /srv/files
$ sudo chown -R someguest:guestsftp /srv/files
```

Higher level folder (`/srv`) must be owned by root, and the guest folder (`/srv/files`) must be owned by guest user and its group.

Switch to the built-in SFTP server (*needed for `chroot` functionality*):

``` sh
$ sudo nano /etc/ssh/sshd_config

Subsystem sftp internal-sftp
```

In the same config add a section for the `guestsftp` group in the very end of the file:

``` conf
Match Group guestsftp
    # force the connection to use SFTP and chroot to the required directory
    ForceCommand internal-sftp -d /files
    ChrootDirectory /srv
    # disable tunneling
    PermitTunnel no
    # disable authentication agent
    AllowAgentForwarding no
    # disable TCP forwarding
    AllowTcpForwarding no
    # disable X11 forwarding
    X11Forwarding no
    # if passwords are disabled upper in the config, so only keys are allowed, enable them here
    # but actually do consider using keys
    #PasswordAuthentication yes
```

Restart the server:

``` sh
$ sudo systemctl restart sshd.service
```

Now `someguest` user (*or any other user from `guestsftp` group*) will be able to connect to server via SFTP (*using his username and password/key*). He will have access only to `/srv/files/`, and he will not be able to login via SSH.

### X11 forwarding

If you are connecting from Mac OS, then you need to install [XQuartz](https://xquartz.org).

``` sh
$ ssh -XY yourname@some-server
```

If it doesn't work like this:

``` sh
Warning: No xauth data; using fake authentication data for X11 forwarding.
```

or like this:

``` sh
X11 connection rejected because of wrong authentication.
```

Then on remote host:

``` sh
$ grep X11Forwarding /etc/ssh/sshd_config
X11Forwarding yes

$ ls -l ~/.Xauthority

$ echo $DISPLAY
$ xauth list

$ touch ~/.Xauthority
$ chmod 600 ~/.Xauthority

$ xauth add $DISPLAY - `mcookie`

$ xauth nextract ~/xcookie $DISPLAY
```

And on local machine:

``` sh
$ scp yourname@some-server:~/xcookie ~/xcookie
$ xauth nmerge ~/xcookie
$ xauth list
```

and also in your local `~/.ssh/config`:

``` sh
Host some-server
HostName 216.18.168.16
ForwardX11 yes # this might be also needed on all proxy-jump hosts
IdentityFile ~/.ssh/some-server
User yourname
```

More details: <https://unix.stackexchange.com/a/412689/254512>

If you still get `rejected because of wrong authentication`, then check what you have in `~/.ssh/` on remote host, such as `~/.ssh/rc` or similar files. Apparently, if these are present, then `xauth` isn't used, and in my case I did have `~/.ssh/rc` file and it was empty, so I deleted it, reconnected, and then I got X11 working.

If some applications work, but some don't, then set environment variable before launching them (*or via `export`*):

``` sh
$ XAUTHORITY=$HOME/.Xauthority firefox
```

### Authentication with 2FA

Your server might have 2FA enabled, for example:

``` sh
$ ssh yourname@some-server
muxclient: master hello exchange failed

  This host utilizes two-factor authentication via Microsoft Azure.
  If you are using the Microsoft Authenticator mobile application, you
  should have received a notification which you need to accept in
  order to proceed. If you are using One Time Password (OTP) codes,
  enter the OTP code now.

(yourname@some-server) Enter Your Microsoft verification code
```

and it very soon will drive you crazy to enter this fucking code every time. Fortunately, you can cache it:

``` sh
Host some-server
HostName 216.18.168.16
IdentityFile ~/.ssh/some-server
User yourname
ControlMaster auto
ControlPath ~/.ssh/%r@%h:%p # /tmp/%r@%h:%p 
ControlPersist 10m
```

Also [here](https://documentation.sigma2.no/getting_help/two_factor_authentication.html#do-i-have-to-use-two-factor-authentication-every-single-time-i-log-in-or-connect-to-betzy-fram-or-saga) they suggest to use `-fN`:

``` sh
$ ssh -fN some-server
```

### Home folder is encrypted

<https://askubuntu.com/a/882379/423703>

If your home folder on the host is encrypted, then it will be impossible to connect to that host via SSH until you log-in locally/directly on that host (*for example via GUI login, if it is a desktop*).

To overcome this you need to move your `authorized_keys` out of your home folder to an unencrypted path, for example:

``` sh
$ sudo mkdir /etc/ssh/$USER
$ sudo mv $HOME/.ssh/authorized_keys /etc/ssh/$USER/
$ sudo chown -R $USER:$USER /etc/ssh/$USER
$ sudo chmod 755 /etc/ssh/$USER
$ sudo chmod 644 /etc/ssh/$USER/authorized_keys
```

Then edit SSH config to point it to the new location:

``` sh
$ /etc/ssh/sshd_config
```
``` ini
#AuthorizedKeysFile     .ssh/authorized_keys .ssh/authorized_keys2
# it is important(?) to add the default paths too
AuthorizedKeysFile /etc/ssh/%u/authorized_keys .ssh/authorized_keys .ssh/authorized_keys2
```

### It's a laptop, and the lid is closed

<https://askubuntu.com/a/1131749/423703>

The system might be configured to get suspended when the lid is closed, so your laptop/host won't be accepting network connections. Disable that in the login config:

``` sh
$ sudo nano /etc/systemd/logind.conf
```
``` ini
#HandleHibernateKey=hibernate
HandleLidSwitch=ignore
#HandleLidSwitchExternalPower=suspend
```

and restart the system.
