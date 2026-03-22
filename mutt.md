# mutt

- <http://mutt.org/>
- <https://wiki.archlinux.org/title/mutt>
- <https://www.cs.uky.edu/docs/users/mutt-cheat-sheet.html>
- <https://gideonwolfe.com/posts/workflow/neomutt/intro/>

It should all be the same for [NeoMutt](https://neomutt.org/) too.

<!-- MarkdownTOC -->

- [Keyboard](#keyboard)
    - [Delete all e-mails in folder](#delete-all-e-mails-in-folder)
- [Account passwords from PGP-encrypted file](#account-passwords-from-pgp-encrypted-file)
- [Viewing HTML](#viewing-html)
- [Debugging](#debugging)
- [Errors](#errors)
    - [No authenticators available](#no-authenticators-available)

<!-- /MarkdownTOC -->

## Keyboard

- `$` - sync mailbox (apply changes, like deleting messages)
- `=` - go to the first item in the mailbox/list
- `*` - go to the last item in the mailbox/list
- `q` - exit the view or the mutt itself

### Delete all e-mails in folder

1. Press `SHIFT` + `D`, input prompt for filter will open;
3. Type `~A`, press `Enter`;
4. All the e-mails will be marked for deletion. Press `$` (`SHIFT` + `4`) to execute the action.

## Account passwords from PGP-encrypted file

``` sh
$ cd /tmp
$ nano ./pwds
```
``` sh
set my_pwd_ololo = nfSas3SF#f54snCs
set my_pwd_some = cm!jj1i495sfdsgs
```

Encrypt the file with PGP and move it to mutt folder:

``` sh
$ gpg --recipient YOUR-EMAIL-FROM-PGP-KEYCHAIN --encrypt ./pwds
$ rm pwds
$ mv pwds.gpg ~/.mutt
```

Now you can read decrypt and read those passwords and refer to them in your configs like this:

``` sh
source "gpg -dq ~/.mutt/pwds.gpg |"
set imap_pass = $my_pwd_ololo
```

Your PGP tool will be asking you for master password, so you might want to save it in the system keychain.

## Viewing HTML

Install **lynx**, set UTF-8 for character set: <http://tolik-punkoff.com/2016/10/29/reshenie-problemy-otobrazheniya-kirillitsy-v-lynx/>. And then:

``` sh
$ nano ~/.mailcap
```
```
text/html; "/Users/vasya/Applications/Firefox\ Developer\ Edition.app/Contents/MacOS/firefox" '%s'; needsterminal;
text/html; lynx -dump '%s' | more; nametemplate=%s.html; copiousoutput;
```

and

``` sh
$ nano ~/.muttrc
```
```
auto_view text/html
alternative_order text/plain text/enriched text/html
```

Open an e-mail. You'll get HTML processed by **lynx**. To view it in Firefox, press `v` for list of attachments, choose HTML attachment and press `m` to send it to mailcap (*and Firefox*).

## Debugging

``` sh
$ neomutt -d2 -l ~/Desktop/mutt.debug
```

Do some stuff, check the logs:

``` sh
$ less ~/Desktop/mutt.debug0
```

Next time it will be `~/Desktop/mutt.debug1` and so on.

## Errors

### No authenticators available

- <https://groups.google.com/forum/#!topic/qubes-users/hL_UGJo1B9s>
- <https://stackoverflow.com/a/56610613/1688203>

``` sh
$ brew install cyrus-sasl
```

Or maybe add in `muttrc`:

``` sh
# if some account don't work, giving errors that there are no authenticators
# but it is also possible that you just provided incorrect credentials
set smtp_authenticators = "gssapi:login"
```
