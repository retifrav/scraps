## PGP

<!-- MarkdownTOC -->

- [Inspect a key file](#inspect-a-key-file)
- [List all the keys and e-mails](#list-all-the-keys-and-e-mails)
- [Generate key](#generate-key)
- [Signing](#signing)
    - [Signing files](#signing-files)
    - [Signing plain text](#signing-plain-text)
- [Edit key](#edit-key)
- [Encrypt a folder with PGP](#encrypt-a-folder-with-pgp)
- [Decrypt text from a file](#decrypt-text-from-a-file)

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

### Signing

#### Signing files

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

#### Signing plain text

``` sh
$ cat ./plaintext.txt
# Some

Thing and then another.

$ gpg --clearsign -u YOUR-KEY-FINGERPRINT ./plaintext.txt

$ cat ./plaintext.txt.asc
-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

# Some

Thing and then another.
-----BEGIN PGP SIGNATURE-----

iQJPBAEBCAA5FiEEYIL81HbRABDPR/aZdkNn6AactosFAmqkOTAbFIAAAAAABAAO
bWFudTIsMi41KzEuMTIsMCwzAAoJEHZDZ+gGnLaLXSEP/0VeYoYvZTOa8gty5sqz
iUQcJvgDX8h6W6IXYDclKz9Bg/iDQstyyMvsfwIvGV8DTTDbdKclfPcU2OaAx+Cd
prrV6dWsVGj3rSi66IDqQhIpugCD+2trXt0SWsSAGEyAMCAJZoXc8jw5/Fb5EgOh
dLbAvKphgFy5Jt9ZEYu16RIXmgkTP7Yu5Zqgx2j3ZDvgbRSugSEiwSGsx/0qW5US
ahv6vhzaQL1q+gWL4mXhix3taKbnidmu0K8hZ3bff6EbPgpKhB6WQV5tPQRQxL6U
UTcbNIOHdwFl9Axq9o35wiOr+e2iUQotG1UsWziqq5o2efFDNEmsPLrCucgXWFss
c+56Zx8fo9VxLcHGv7QyW1AmuNRMXjq0RfTjTei8si7Kc+iqBrG5EUsWGDMecDky
cRKTSSo+RyDFCtrwnpM7Qd0AA8zo0kkAMJph+bh8MphDEJPcuP43aIW4CeaVjr5F
9z0FrIZkltQD/aaVM0Jp6jjX7IGDz4ixnPRMpNnRiNZgsKUoNW/bRb2jWI9PqEsQ
1OnV9Imn6aasEEmuXNxXBYyghwj6via0S8d07CJ87H861iO8+S3d5OyZWozMpHDz
qImaQNvUBRPk1FGXUdRrUTFolplGXOy37pWffQII3T/5yY93whPSTEsCJWvpDhdp
FCt+6y8+1NPF2m77lmSlZCGb
=EP9w
-----END PGP SIGNATURE-----

$ gpg --verify ./plaintext.txt.asc
gpg: Signature made Fri Sep 11 19:24:00 2026 CEST
gpg:                using RSA key 6082FCD476D10010CF47F699764367E8069CB68B
gpg: Good signature from ...

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

### Decrypt text from a file

First, who it is for:

``` sh
$ gpg --list-packets ./email.pgp.asc
```

And then decryption:

``` sh
$ gpg --output ./plaintext.txt --decrypt ./email.pgp.asc
```
