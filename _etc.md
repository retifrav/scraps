# etc

Various uncategorized things that are not specific to a particular platform and do not deserve their own file (*yet*).

<!-- MarkdownTOC -->

- [Base64](#base64)
    - [Encode](#encode)
    - [Decode](#decode)
    - [Image to Base64 and back](#image-to-base64-and-back)
- [OpenSSL](#openssl)
    - [Check certificate dates](#check-certificate-dates)
    - [x509 certificate](#x509-certificate)
        - [For TLS/SSL HTTPS](#for-tlsssl-https)
        - [For .NET project persisting keys](#for-net-project-persisting-keys)
    - [SHA384 hash for subresource integrity](#sha384-hash-for-subresource-integrity)
- [awk](#awk)
    - [Filter a list](#filter-a-list)
- [pandoc](#pandoc)
    - [PDF from Markdown](#pdf-from-markdown)
- [Convert a text file from one encoding to another](#convert-a-text-file-from-one-encoding-to-another)
- [Diff and patch files](#diff-and-patch-files)
    - [All files in two directories](#all-files-in-two-directories)
- [Working with FTP](#working-with-ftp)
    - [ftp](#ftp)
    - [lftp](#lftp)
    - [Multiline RegEx replace](#multiline-regex-replace)

<!-- /MarkdownTOC -->

## Base64

### Encode

Original string: `s0me-pa$$w0rd-01010`.

Several ways to encode it to Base64:

```
$ echo -n "s0me-pa$$w0rd-01010" | base64
czBtZS1wYTE3MTF3MHJkLTAxMDEw

$ echo "s0me-pa$$w0rd-01010" | tr -d "\n" | base64
czBtZS1wYTE3MTF3MHJkLTAxMDEw

$ python -c "import base64; print(base64.b64encode(b's0me-pa$$w0rd-01010').decode('ascii'))"
czBtZS1wYTE3MTF3MHJkLTAxMDEw

$ echo -n s0me-pa$$w0rd-01010 | openssl enc -base64 -A
czBtZS1wYTE3MTF3MHJkLTAxMDEw
```

Note that Node, however, produces a different result:

```
$ node -e "require('readline') .createInterface({input:process.stdin,output:process.stdout,historySize:0}) .question('PAT> ',p => { b64=Buffer.from(p.trim()).toString('base64');console.log(b64);process.exit(); })"
PAT> s0me-pa$$w0rd-01010
czBtZS1wYSQkdzByZC0wMTAxMA==
```

And actually this is the only correct result, because original string contains special characters and those need to be escaped before encoding, for example:

```
echo -n "s0me-pa\$\$w0rd-01010" | base64
czBtZS1wYSQkdzByZC0wMTAxMA==
```

And since there doesn't seem to be a (*convenient enough*) way of handling such escaping automatically, better to put such strings into a file (`tmp.txt`) and encode the file:

```
$ echo -n $(cat tmp.txt) | base64
czBtZS1wYSQkdzByZC0wMTAxMA==
```

### Decode

Encoded string: `czBtZS1wYSQkdzByZC0wMTAxMA==`.

Several ways to decode it:

```
$ base64 -d <<< "czBtZS1wYSQkdzByZC0wMTAxMA=="
s0me-pa$$w0rd-01010

$ python -c "import base64; print(base64.b64decode(b'czBtZS1wYSQkdzByZC0wMTAxMA==').decode('ascii'))"
s0me-pa$$w0rd-01010
```

### Image to Base64 and back

Image (*or any other file*) can be encoded into a Base64 string:

``` sh
$ base64 -w0 some.png > encoded.txt
```

or decoded back:

``` sh
$ cat ./file-with-encoded-string.txt | base64 -d > ./image.png
```

## OpenSSL

### Check certificate dates

HTTPS:

``` sh
$ echo | openssl s_client -connect decovar.dev:443 2>&1 | openssl x509 -noout -dates
```

or STARTTLS:

``` sh
$ echo | openssl s_client -connect outlook.office365.com:587 -starttls smtp 2>&1 | openssl x509 -noout -dates
```

### x509 certificate

#### For TLS/SSL HTTPS

Such as for Synology DSM web-interface:

``` sh
$ openssl req -new -newkey rsa:4096 -x509 -sha256 -days 1111 -nodes \
    -out synology-certificate.crt -keyout synology-key.key
```

or just for localhost HTTPS:

```
$ openssl req -x509 -newkey rsa:2048 -nodes -sha256 \
    -subj '/CN=localhost' -extensions EXT -config <( \
    printf "[dn]\nCN=localhost\n[req]\ndistinguished_name = dn\n[EXT]\nsubjectAltName=DNS:localhost\nkeyUsage=digitalSignature\nextendedKeyUsage=serverAuth") \
    -out localhost.crt -keyout localhost.key
```

#### For .NET project persisting keys

To be used in a .NET Core project for [PersistKeysToFileSystem](https://docs.microsoft.com/en-us/aspnet/core/security/data-protection/implementation/key-storage-providers?view=aspnetcore-3.1&tabs=visual-studio#file-system).

```
$ openssl req -x509 -newkey rsa:4096 -sha256 -keyout key.pem -out cert.pem -days 365 -nodes -subj "/CN=YOUR-DOMAIN"
$ openssl pkcs12 -inkey key.pem -in cert.pem -export -out cert.pfx -passout pass:YOUR-PASSWORD
```

### SHA384 hash for subresource integrity

<https://developer.mozilla.org/en-US/docs/Web/Security/Subresource_Integrity>

Calculate the hash:

``` sh
$ openssl dgst -sha384 -binary ./js/search.js | openssl base64
KtaR/jXih+79pUu1lBvA9CKrxRwT/MzNwCg9aWj4VkgdWWjsV3IlZqvCe/Z99bNu
```

Use it for `integrity` attribute:

``` html
<script src="/js/search.js" integrity="sha384-KtaR/jXih+79pUu1lBvA9CKrxRwT/MzNwCg9aWj4VkgdWWjsV3IlZqvCe/Z99bNu"></script>
```

## awk

### Filter a list

To show only open ports for IPv4 protocol:

```
$ netstat -lntup | awk '{ if (($1 == "tcp") && ($6 == "LISTEN")) { print $1, " | ", $4, " | ", $6 } }'
tcp  |  0.0.0.0:22  |  LISTEN
tcp  |  127.0.0.1:631  |  LISTEN
tcp  |  127.0.0.1:25  |  LISTEN
tcp  |  127.0.0.1:8890  |  LISTEN
tcp  |  127.0.0.1:49020  |  LISTEN
```

## pandoc

### PDF from Markdown

``` sh
$ pandoc /path/to/some.md \
    --from=markdown-implicit_figures --to=pdf --pdf-engine=xelatex \
    --variable mainfont='Verdana' --variable monofont='Input Mono' \
    --variable fontsize='10pt' --variable monofontoptions='Scale=0.9' \
    --variable urlcolor='[HTML]{0000ff}' --variable pagestyle=empty \
    --variable margin-left='20mm' --variable margin-right='20mm' \
    --variable margin-top='20mm' --variable margin-bottom='20mm' \
    --highlight-style haddock \
    --output ./report.pdf
```

## Convert a text file from one encoding to another

``` sh
$ iconv -f windows-1251 -t utf-8 ./doctor-who-s07e02-dinosaurs-on-a-spaceship.srt \
    -o ./doctor-who-s07e02-dinosaurs-on-a-spaceship.srt
```

On Mac OS there is no `-o`, so you'll have to:

``` sh
$ iconv -f windows-1251 -t utf-8 ./doctor-who-s07e02-dinosaurs-on-a-spaceship.srt > ./out.srt \
    && mv ./out.srt ./doctor-who-s07e02-dinosaurs-on-a-spaceship.srt
```

## Diff and patch files

Suppose you got `1.txt` from someone. You created a copy of it (`2.txt`) and made some changes there. You can now compare these two files and create a patch based on their differences:

```
$ diff 1.txt 2.txt > 3.patch
```

Send your patch to the person with the original `1.txt`. He will then be able to apply your patch and get your `2.txt`:

```
$ patch 1.txt 3.patch
```

If he gets errors like:

```
Hunk #1 FAILED at 1 (different line endings).
Hunk #2 FAILED at 3 (different line endings).
Hunk #3 FAILED at 6 (different line endings).
Hunk #4 FAILED at 8 (different line endings).
```

then after running this:

```
$ dos2unix 3.patch
```

the patch should be able to apply.

### All files in two directories

``` sh
$ diff -uraBN /home/vasya/downloads/qt-everywhere-src-5.15.2 /home/vasya/programs/qt/src/5.15.2 > some-fixes.patch
```

Here left/first path is original Qt sources and right/second path is modified sources.

Then to apply the patch later:

``` sh
$ patch --dry-run -d / -p0 -i /path/to/some-fixes.patch
$ patch -d / -p0 -i /path/to/some-fixes.patch
```

The files that are to be patched need to be where the *left*/*first* file path inside patch file points to. So if you are doing this one a different machine, ensure that original sources are located in that path (*or modify the paths in the patch file*).

Also note that `-d /` and `-p0` arguments are important, without them it will be crying that it can't file files. You can also set `-p1` there - doesn't seem to make any difference. Some say that the command needs to be run from the root path, but I just ran it from `/tmp` and it was all good anyway.

Works in Git BASH on Windows too.

## Working with FTP

### ftp

```
$ cd /path/to/where/you/want/to/download/files/

$ ftp some.server.io
Name (some.server.io:name): YOUR-LOGIN
331 Password required for YOUR-LOGIN
Password: YOUR-PASSWORD
230 User YOUR-LOGIN logged in

ftp> ls
ftp> cd some-folder
ftp> ls
ftp> hash
Hash mark printing on (1024 bytes/hash mark).
ftp> tick
Hash mark printing off.
Tick counter printing on (10240 bytes/tick increment).
ftp> get some-file.mp4
```

### lftp

[lftp](https://en.wikipedia.org/wiki/Lftp) supports [FTPS](https://en.wikipedia.org/wiki/FTPS).

Create a config file:

```
nano ~/.lftprc
```
```
set ftps:initial-prot P
set ftp:ssl-allow true
set ftp:ssl-force true
set ftp:ssl-protect-data false
set ftp:ssl-protect-list true
set ssl:verify-certificate false

open some.server:21
user USERNAME PASSWORD
```

Connect and download some file:

```
$ lftp
lftp USERNAME@some.server:~> ls
drwxr-xr-x   2 USERNAME  USERNAME         6 Apr 21  2016 download
drwxr-x---  25 USERNAME  USERNAME     12288 Sep 21 18:49 files
lftp USERNAME@some.server:/> cd files
lftp USERNAME@some.server:/files> get something.mkv -o /storage/hdd/tv/
`something.mkv' at 231331800 (35%) 10.52M/s eta:38s [Receiving data]
```

### Multiline RegEx replace

<https://unix.stackexchange.com/a/26289>

Original file:

``` json
{
    "name": "some",
    "version": "1.2.3",
    "description": "Some thing",
    "homepage": "https://example.com/",
    "dependencies":
    [
        "png",
        "proj",
        "rapidxml"
    ],
    "overrides":
    [
        {
            "name": "png",
            "version": "1.6.38"
        },
        {
            "name": "proj",
            "version": "7.2.1"
        },
        {
            "name": "rapidxml",
            "version": "1.13.0"
        }
    ]
}
```

Replacement command:

``` sh
$ perl -0777 -i.original -pe 's/{\s*"name":\s*"proj",\s*"version":\s*"7\.2\.1"\s*}/{ "name": "proj", "version": "9.3.1" }/' ./vcpkg.json
```

Result:

``` sh
$ diff ./vcpkg.json ./vcpkg.json.original
18c18,21
<         { "name": "proj", "version": "9.3.1" },
---
>         {
>             "name": "proj",
>             "version": "7.2.1"
>         },
```

In order not to create a backup copy of the original file, replace `-i.original` with just `-i`.
