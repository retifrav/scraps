## PGP

<!-- MarkdownTOC -->

- [List all the keys and e-mails](#list-all-the-keys-and-e-mails)
- [Signing files](#signing-files)
- [Edit key](#edit-key)

<!-- /MarkdownTOC -->

### List all the keys and e-mails

```
$ gpg --list-secret-keys --keyid-format LONG
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
gpg: Good signature from "retif <user@domain.com>" [ultimate]

$ echo $?
0

$ gpg --verify some.sig another.exe
gpg: Signature made 02/15/21 14:16:55 W. Europe Standard Time
gpg:                using RSA key KEY-ID
gpg: BAD signature from "retif <user@domain.com>" [ultimate]

$ echo $?
0
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
