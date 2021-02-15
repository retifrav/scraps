## PGP

<!-- MarkdownTOC -->

- [List all the keys and e-mails](#list-all-the-keys-and-e-mails)
- [Signing files](#signing-files)

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
