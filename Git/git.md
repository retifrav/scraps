## Git

Manual that you will never read: https://git-scm.com/book/en/

- [Settings](#settings)
- [Commit](#commit)
- [Log](#log)
- [Checkout or reset](#checkout-or-reset)
- [Remotes](#remotes)
- [Submodules](#submodules)
  - [Remove submodule](#remove-submodule)
- [Set identity and PGP](#set-identity-and-pgp)
- [Change the author of past commits](#change-the-author-of-past-commits)
- [GitHub via SSH](#github-via-ssh)
- [Remove the latest local commits](#remove-the-latest-local-commits)
- [List commits with count number](#list-commits-with-count-number)
- [File changes history](#file-changes-history)

### Settings

You have global settings for all repositories using ~—global~ option, and without it only local config will be affected (obviously, you need to be inside this repository’s directory):

Set user name globally:

``` bash
git config --global user.name “Ivan Petrov“
```

Set user name only for the current repository:

``` bash
git config user.name “Ivan Petrov“
```

Contents of local config:

``` bash
cat .git/config
```

Compilation of global config and local config contents:

``` bash
git config --list
```

Get user information or any other config values:

``` bash
git config user.name
git config user.email
```

Set user information or any other config values:

``` bash
git config user.name “Ivan Petrov“
git config user.email "ivan@example.org"
```

You can edit global settings file:

``` bash
git config --global --edit
```

### Commit

Status of the current repository. Shows untracked files, uncommitted changes, current branch and commit, etc:

``` bash
git status
git status -s # short version
```

If some files are not tracked - your commits will not include them. Start to track files (excluding those listed in `.gitignore`):

``` bash
git add *
```

* the wildcard `*` is used to add all the files in the directory.

Commit changes with the commit message passed after `-m` option:

``` bash
git commit -m "First commit"
```

Push changes to remote:

``` bash
git push # pushes your commits from `master` branch to the default remote repository (`origin/master`)
git push SomeRepo someBranch # pushes commits from `someBranch` to `SomeRepo` remote repository
```

### Log

History of commits:

``` bash
git log
git log --stat # detailed history of commits. You can scroll the output and use `q` for exit
git log --oneline # formatted history (`short`, `full`, `format`)
git log --since=2.weeks # history of commits for the past 2 weeks
```

You can customize `log` format. Default output:

```
$ git log

commit ebea4e36c342346174001da477f30bb5189f1a02 (HEAD -> master, server/master)
Author: retif <retifrav@gmail.com>
Date:   Sun Sep 22 00:05:11 2019 +0200

    Cider links

commit 50dee2c41d4af54fe4316253a18b22d4e890920c
Author: retif <retifrav@gmail.com>
Date:   Sat Sep 21 23:47:02 2019 +0200

    Ciders hovers

commit 873b565dbdd3ed62adafae981bd58bc13bc58183
Author: retif <retifrav@gmail.com>
Date:   Sat Sep 21 20:12:22 2019 +0200

    Update
```

Now add this to `~/.gitconfig`:

```
[log]
        date = relative
[format]
        pretty = format:%C(yellow)%h %Cblue%>(12)%ad %Cgreen%<(7)%aN%Cred%d %Creset%s
```

Check again:

```
$ git log
ebea4e3   2 days ago retif   (HEAD -> master, server/master) Cider links
50dee2c   2 days ago retif   Ciders hovers
873b565   3 days ago retif   Update
```

### Checkout or reset

Switch to a specific commit:

```
git checkout COMMIT-HASH
```

When it fails, fuck everything and just switch to the bloody commit:

```
git reset --hard COMMIT-HASH
```

### Remotes

List all remotes:

``` bash
git remote -v
```

Check if there is anything new on any of remotes:

```
git remote update
```

Also:

```
git fetch --all
```

Or just the current branch:

```
git fetch
```

Check how far you are behind:

```
git status
```

Download commits from the current remote:

```
git pull
```

### Submodules

#### Remove submodule

If you want to move submodule source to the main repository. Make a copy first.

``` bash
git submodule deinit relative-path-to-submodule
git rm relative-path-to-submodule
git commit -m "Removed submodule"
rm -rf .git/modules/relative-path-to-submodule
```

And then copy those files into repository and commit changes.

### Set identity and PGP

Get your key information.

``` bash
$ gpg --list-secret-keys --keyid-format LONG

/Users/ivan/.gnupg/pubring.kbx
-------------------------------
sec   rsa4096/9BS46220013CA6BA 2017-12-29 [SC] [expires: 2018-12-29]
      41F3AF02C1BA14N2157352469BS46220013CA6BA
uid                 [ultimate] Ivan Petrov <ivan@example.org>
ssb   rsa4096/0H6F00B15908023C 2017-12-29 [E] [expires: 2018-12-29]
```

Clone the repository, go inside.

``` bash
$ git config user.name "Ivan Petrov"
$ git config user.email "ivan@example.org"
$ git config user.signingkey 9BS46220013CA6BA

$ git config --global commit.gpgsign true
$ git config --global gpg.program /usr/local/bin/gpg
```

### Change the author of past commits

``` bash
git filter-branch --env-filter '
OLD_EMAIL="OLD-AUTHOR@example.com"
NEW_NAME="New Author"
NEW_EMAIL="NEW-AUTHOR@ololo.org"

if [ "$GIT_COMMITTER_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags
```

### GitHub via SSH

``` bash
cd ~/.ssh/
ssh-keygen -o -t rsa -C "your.email@example.com" -b 4096
```

Enter the file name, for example `id_rsa_github_yourname`. Two keys will be generated:

* public: `id_rsa_github_yourname.pub`;
* private: `id_rsa_github_yourname`.

Change permissions just in case:

``` bash
chmod 400 ~/.ssh/id_rsa_github_yourname*
```

Copy contents of `id_rsa_github_yourname.pub` and save it to your [GitHub account](https://github.com/settings/keys).

Now add the key to keychain and test it:

``` bash
ssh-add ~/.ssh/id_rsa_github_yourname
ssh-add -l
ssh -T git@github.com
```

Also don't forget to add remote repository using its SSH link and not HTTP. For example: `ssh://git@github.com:retifrav/scraps.git`

### Remove the latest local commits

Check what you have:

``` bash
$ git log --oneline --abbrev-commit

37su948 Some stupid change you want to delete
l4h5cs3 Another change
148gd35 Some change
dm36c8g First commit
```

You want to delete the latest commit (`37su948`). First change the HEAD:

```
git rebase -i HEAD~2
```

The text editor will open. Now simply delete the line with this commit (`37su948 Some stupid change you want to delete`), save the changes and close the editor.

### List commits with count number

```
git log --oneline | nl
```

### File changes history

```
git log -p -- main.cpp
```
