## SSH

<!-- MarkdownTOC -->

- [Generate a new SSH key](#generate-a-new-ssh-key)
- [SSH config example](#ssh-config-example)
- [Ignore changed remote host identification](#ignore-changed-remote-host-identification)
- [Disable SSH passwords](#disable-ssh-passwords)
- [Open a tunnel to some port](#open-a-tunnel-to-some-port)
- [Run a remote command](#run-a-remote-command)
- [Read or source remote encrypted PGP file](#read-or-source-remote-encrypted-pgp-file)
- [Download a file](#download-a-file)
- [SFTP guest access](#sftp-guest-access)

<!-- /MarkdownTOC -->

### Generate a new SSH key

``` sh
$ cd ~/.ssh
$ ssh-keygen -o -t rsa -b 4096 -C "name@example.org"
```

Leave empty password (or whatever) and set the file name. Change permissions for the files: `chmod 600 id_rsa_newkey*`.

### SSH config example

An example of a config section (`~/.ssh/config`) for connecting to some remote host:

``` config
Host mahserver                       # alias for convenience
HostName 216.18.168.16               # actual host address
IdentityFile ~/.ssh/id_rsa_mahserver # SSH key file
User root                            # username
```

### Ignore changed remote host identification

When you have the same device, and you keep switching Linux installations on it, but DHCP gives it the same IP, so your `~/.ssh/known_hosts` is not happy about it.

``` sh
$ ssh -o UserKnownHostsFile=/dev/null root@SOME-IP-ADDRESS
```

And that way the "updated" key will not get saved.

### Disable SSH passwords

So you could connect only by using key.

Your public key should be placed into `~/.ssh/authorized_keys` on the remote host.

Disable SSH passwords:

``` sh
$ sudo nano /etc/ssh/sshd_config
```

In this file uncomment and change to `no` the following:

``` sh
ChallengeResponseAuthentication no
PasswordAuthentication no
UsePAM no
```

If you would like to allow some user to still use the password, then in the end of file:

``` config
Match User vasya
    PasswordAuthentication yes
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

### Download a file

From remote host to your local machine:

``` sh
$ sftp username@host:/data/backups/some.tar.gz ~/downloads/
```

### SFTP guest access

Based on <https://passingcuriosity.com/2014/openssh-restrict-to-sftp-chroot/>.

When needed to transfer big amounts of data, cloud uploads might not be suitable because of possible disconnections, non-resumable downloads, auto-generated URLs and so on. In that case you can give a person SFTP access to your server.

Add a new group and user:

``` sh
$ sudo addgroup guestsftp
$ sudo adduser --disabled-password --ingroup guestsftp someguest
```

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
```

Restart the server:

``` sh
$ sudo systemctl restart sshd.service
```

Now `someguest` user (*or any other user from `guestsftp` group*) will be able to connect to server via SFTP and have access only to `/srv/files/`. And he will not be able to login via SSH.
