## PGP

<!-- MarkdownTOC -->

- [Inspect a key file](#inspect-a-key-file)
- [List all the keys and e-mails](#list-all-the-keys-and-e-mails)
- [Generate key](#generate-key)
- [Signing files](#signing-files)
- [Edit key](#edit-key)
- [Encrypt a folder with PGP](#encrypt-a-folder-with-pgp)

<!-- /MarkdownTOC -->

### Inspect a key file

``` sh
$ gpg --show-keys ./some.asc
pub   rsa4096 2022-09-10 [SC]
      6082FCD476D10010CF47F699764367E8069CB68B
```

### List all the keys and e-mails

``` sh
$ gpg --list-secret-keys --keyid-format LONG
```

### Generate key

``` sh
$ gpg --full-generate-key
$ gpg --list-secret-keys --keyid-format=long
$ gpg --armor --export KEY-ID > ~/yourkey-public.asc
$ gpg --armor --export-secret-keys KEY-ID > ~/yourkey-private.asc
```

### Signing files

Sign a file with specific key:

```
$ gpg --sign --local-user KEY-ID --output some.sig --detach-sig some.exe
```

Now check the signature:

```
$ gpg --verify some.sig some.exe
gpg: Signature made 02/15/21 14:16:55 W. Europe Standard Time
gpg:                using RSA key KEY-ID
gpg: Good signature from "user <user@domain.com>" [ultimate]

$ echo $?
0

$ gpg --verify some.sig another.exe
gpg: Signature made 02/15/21 14:16:55 W. Europe Standard Time
gpg:                using RSA key KEY-ID
gpg: BAD signature from "user <user@domain.com>" [ultimate]

$ echo $?
1
```

### Edit key

Say you've created a key just for signing (`[SC]`) and now you want to extend it with encrypting capabilities:

``` sh
$ gpg --edit-key K3B36613975DB722

gpg> change-usage
Changing usage of the primary key.

Possible actions for a RSA key: Sign Certify Encrypt Authenticate
Current allowed actions: Sign Certify

   (S) Toggle the sign capability
   (E) Toggle the encrypt capability
   (A) Toggle the authenticate capability
   (Q) Finished

Your selection? E

Possible actions for a RSA key: Sign Certify Encrypt Authenticate
Current allowed actions: Sign Certify Encrypt

   (S) Toggle the sign capability
   (E) Toggle the encrypt capability
   (A) Toggle the authenticate capability
   (Q) Finished

Your selection? Q
```

### Encrypt a folder with PGP

Pack the folder into a single archive file (*no compression*):

``` sh
$ tar -C /path/to/folder/to/encrypt -cf archName.tar .
```

If you haven't already, export your public key on local machine (*where your keychain is*), transfer it to the server and import it there:

``` sh
$ gpg --import your-public-key.asc 
```

Now you can encrypt the archive:

``` sh
$ gpg --encrypt --recipient your@email.com archName.tar
```
