## Git

Manual that you will never read: https://git-scm.com/book/en/

<!-- MarkdownTOC -->

- [Settings](#settings)
    - [Different user identity for different paths](#different-user-identity-for-different-paths)
- [Cloning](#cloning)
    - [Shallow clone](#shallow-clone)
- [Commits](#commits)
    - [Status](#status)
    - [Tracking](#tracking)
    - [Committing](#committing)
- [History](#history)
    - [Log](#log)
    - [History of a particular file](#history-of-a-particular-file)
    - [History of a particular diapason of lines](#history-of-a-particular-diapason-of-lines)
    - [Inspect a single commit](#inspect-a-single-commit)
    - [Get the date of the commit](#get-the-date-of-the-commit)
    - [List authors sorted by number of commits](#list-authors-sorted-by-number-of-commits)
    - [Plot author contributions per year](#plot-author-contributions-per-year)
    - [Reset repository history](#reset-repository-history)
- [Checkout or reset](#checkout-or-reset)
    - [Discard local changes](#discard-local-changes)
    - [Checkout specific commit](#checkout-specific-commit)
    - [Delete untracked](#delete-untracked)
- [Branches](#branches)
    - [List branches](#list-branches)
    - [Switch to some branch](#switch-to-some-branch)
    - [Delete branch](#delete-branch)
    - [Rename main to master on GitHub](#rename-main-to-master-on-github)
- [Remotes](#remotes)
- [Submodules](#submodules)
    - [Remove submodule](#remove-submodule)
- [Stash](#stash)
    - [Add current uncommitted changes to stash](#add-current-uncommitted-changes-to-stash)
    - [Apply the latest stash](#apply-the-latest-stash)
    - [List existing stashes](#list-existing-stashes)
    - [Delete all the stashes](#delete-all-the-stashes)
- [Set identity and PGP](#set-identity-and-pgp)
- [Change the author of past commits](#change-the-author-of-past-commits)
- [GitHub via SSH](#github-via-ssh)
- [Remove the last commit](#remove-the-last-commit)
- [List commits with count number](#list-commits-with-count-number)
- [Tags](#tags)
    - [List tags](#list-tags)
    - [Check if commit has tags](#check-if-commit-has-tags)
    - [Get any last tag down the current branch](#get-any-last-tag-down-the-current-branch)
    - [Absolutely the last tag across all the branches](#absolutely-the-last-tag-across-all-the-branches)
- [Patches](#patches)
- [Bare repository](#bare-repository)
    - [Create and clone](#create-and-clone)
    - [Copy files with a hook](#copy-files-with-a-hook)

<!-- /MarkdownTOC -->

### Settings

You have global settings for all repositories using ~—global~ option, and without it only local config will be affected (obviously, you need to be inside this repository’s directory):

Set user name globally:

``` bash
git config --global user.name "Ivan Petrov"
```

Set user name only for the current repository:

``` bash
git config user.name "Ivan Petrov"
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
git config user.name "Ivan Petrov"
git config user.email "ivan@example.org"
```

You can edit global settings file:

``` bash
git config --global --edit
```

#### Different user identity for different paths

Just in case, no identity settings in the main `~/.gitconfig`. Add the following include statements there:

``` ini
[includeIf "gitdir:~/code/"]
    path = .gitconfig-neo
[includeIf "gitdir:~/Documents/work/metacortex/"]
    path = .gitconfig-anderson
```

Note that paths are case-sensitive, and on Windows you must have capital disk letters or better yet use `/i` option, for example:

``` ini
[includeIf "gitdir/i:d:/code/"]
    path = .gitconfig-neo
[includeIf "gitdir/i:e:/work/metacortex/"]
    path = .gitconfig-anderson
```

Also the paths need to have a trailing slash.

Then create `~/.gitconfig-neo`:

``` ini
[user]
    name = neo
    email = neo@nebuchadnezzar.net
    signingkey = ONE-PGP-KEY
```

and `~/.gitconfig-anderson`:

``` ini
[user]
    name = Thomas Anderson
    email = thomas.anderson@metacortex.com
    signingkey = ANOTHER-PGP-KEY
```

To verify that it works correctly, run the following from some repository:

``` sh
$ git config --list --show-origin | grep user.
```

### Cloning

#### Shallow clone

You want to get a shallow clone (*limited history of commits*) of your repository:

```
$ git clone --depth 1000 git@your.git.host:your/repository.git
$ cd repository
$ git config --get remote.origin.fetch
+refs/heads/master:refs/remotes/origin/master
```

So you only got `master` branch. But you can add other (*also shallowed*) branches too:

```
$ git remote set-branches --add origin 'some-other-branch'
$ git config --get remote.origin.fetch
+refs/heads/some-other-branch:refs/remotes/origin/some-other-branch

$ git fetch --depth 1000 origin some-other-branch

$ git branch
* master

$ git branch -r
  origin/HEAD -> origin/master
  origin/some-other-branch
  origin/master
```

If you'll need to push from a shallow clone to a different remote repository, it will fail:

```
 ! [remote rejected]   master -> master (shallow update not allowed)
error: failed to push some refs to 'gitlab.your.host:some/project.git'
```

To fix that you'll need to run this first:

``` sh
$ git fetch --unshallow NAME-OF-THE-REMOTE-WHICH-YOU-SHALLOW-CLONNED-FROM
```

### Commits

#### Status

Status of the current repository. Shows untracked files, uncommitted changes, current branch and commit, etc:

``` bash
git status
git status -s # short version
```

#### Tracking

If some files are not tracked - your commits will not include them. Start to track all the files (excluding those listed in `.gitignore`):

```
git add *
```

Or you can track only specific files/folders:

```
git add git add some/folder
```

To remove file from tracking:

```
git rm --cached some.cpp
```

To remove folder from tracking:

```
git rm -r --cached some/folder
```

#### Committing

Commit changes with the commit message passed after `-m` option:

``` bash
git commit -m "First commit"
```

Push changes to remote:

``` bash
git push # pushes your commits from `master` branch to the default remote repository (`origin/master`)
git push SomeRepo someBranch # pushes commits from `someBranch` to `SomeRepo` remote repository
```

### History

#### Log

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

#### History of a particular file

```
$ git log --date=iso --follow ./file/in/repository
```

or:

```
$ git log -p -- ./file/in/repository
```

#### History of a particular diapason of lines

<https://stackoverflow.com/a/19757493/1688203>

With `blame`:

``` sh
$ git blame -L 36,40 -- ./documentation/examples/sdl/CMakeLists.txt
```

or with `log`:

``` sh
$ git log -L 36,40:./documentation/examples/sdl/CMakeLists.txt
```

#### Inspect a single commit

```
$ git show COMMIT-HASH
```

#### Get the date of the commit

```
$ git show --no-patch --no-notes --pretty='%cd' --date=iso COMMIT-HASH
```

#### List authors sorted by number of commits

``` sh
$ git shortlog -sn
```

#### Plot author contributions per year

```
$ git rev-list --no-commit-header --format=%as --author="Vasya Ivanov" HEAD | cut -d- -f1 | feedgnuplot --unset grid --histogram 0 --terminal 'dumb'


  300 +--------------------------------------------------------------------+
      |                ************************************                |
      |                *                                  *                |
  250 |-+              *                                  *              +-|
      |                *                                  *                |
      |                *                                  *                |
      |                *                                  *                |
  200 |-+              *                                  *              +-|
      |                *                                  *                |
      |                *                                  *****************|
  150 |-+              *                                  *              +-|
      |                *                                  *                |
      |                *                                  *                |
  100 |-+              *                                  *              +-|
      |                *                                  *                |
      |                *                                  *                |
      |                *                                  *                |
   50 |-+              *                                  *              +-|
      |                *                                  *                |
      |*****************                 +                *                |
    0 +--------------------------------------------------------------------+
     2019            2019.5             2020            2020.5            2021
```

#### Reset repository history

Just in case, be aware that you will lose your entire repository commits history, both local and remote.

``` sh
$ rm -rf .git

$ git init
$ git add .
$ git commit -m "First commit"

$ git remote add GitHub git@github.com:YOURNAME/YOUREPOSITORY.git
$ git push -u --force GitHub master
```

### Checkout or reset

#### Discard local changes

```
$ git checkout -- .
```

#### Checkout specific commit

```
$ git checkout COMMIT-HASH
```

When it fails, fuck everything and just switch to the bloody commit:

```
$ git reset --hard COMMIT-HASH
```

#### Delete untracked

What will be deleted:

```
$ git clean -d -n
```

Delete untracked files and folders:

```
$ git clean -d -f
```

### Branches

#### List branches

Local:

```
$ git branch
```

Remote:

```
$ git branch -r
```

#### Switch to some branch

```
$ git checkout origin/some-branch
```

#### Delete branch

Local:

```
$ git branch -d test
```

Remote:

```
$ git push origin --delete test
```

#### Rename main to master on GitHub

Credits to SJW-challenged Tower team: <https://www.git-tower.com/learn/git/faq/git-rename-master-to-main/>

In your local repository:

```
$ git branch -m main master
$ git push -u origin master
```

Go to <https://github.com/YOURNAME/YOUR-REPOSITORY/settings/branches> and change default branch to `master`. Then:

```
$ git push origin --delete main
$ git remote set-head origin master
```

Also go to <https://github.com/settings/repositories> and change default branch for new repositories back to `master`.

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

### Stash

#### Add current uncommitted changes to stash

```
git stash
```

#### Apply the latest stash

```
git stash pop
```

That will also delete this stash.

#### List existing stashes

```
git stash list
```

#### Delete all the stashes

```
git stash clear
```

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

### Remove the last commit

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

That will get you 2 commits back. Or you can specify the commit hash:

```
git rebase -i 24a113b
```

The text editor will open. Now simply delete the line with this commit (`37su948 Some stupid change you want to delete`), save the changes and close the editor.

If you have pushed your changes containing this commit, push this new history with force:

```
git push -f origin master
```

On GitLab (and perhaps in some other similar services) you will need to unprotect the branch before doing that (and protect it back afterwards).

### List commits with count number

```
git log --oneline | nl
```

### Tags

#### List tags

List all tags:

```
$ git tag

build-2015.11_44422
build-2015.11_46216
build-2016.03_41007
build-2016.03_41017
build-trunk_41189
build-trunk_41191
...
```

List all tags matching the pattern:

```
$ git tag -l build-trunk*

build-trunk_41189
build-trunk_41191
build-trunk_41193
build-trunk_41200
build-trunk_41213
...
```

#### Check if commit has tags

```
$ git describe --exact-match e6dbcb98ddfa1aef2850fke9ba9f0c65f32f7ebb
```

or

```
$ git describe --exact-match --abbrev=0
```

#### Get any last tag down the current branch

```
$ git describe --tags --abbrev=0 --match "build-*"
```

#### Absolutely the last tag across all the branches

```
$ git describe --tags $(git rev-list --tags --max-count=1)
```

### Patches

If you want to create a patch from your uncommitted changes (*including non-text files such as images*), **stage** everything (*what you want to include to the patch*) first and then from your repository root:

```
$ git diff --cached --binary > some.patch
```

Then the person who you'll send your patch to will be able to apply it in their repository like this:

```
$ git apply ~/Downloads/some.diff
```

### Bare repository

Shortly saying, "bare" repository has no files, only Git stuff. Usually such repositories live on servers and are only meant to be pushed to.

#### Create and clone

``` sh
$ mkdir some && cd $_
$ git init --bare .
```

To clone it via SSH:

``` sh
$ git clone that.server.host:/path/to/repository/on/server
```

#### Copy files with a hook

As one cannot just copy files from a bare repository, and yet he might need to have repository files available on the server lying somewhere nearby.

<https://stackoverflow.com/a/14453711/1688203>

``` sh
$ nano ./some/hooks/post-receive
```
``` sh
#!/bin/bash

GIT_WORK_TREE=/path/where/to/copy/files/to git checkout -f
exit
```
``` sh
$ chmod +x ./some/hooks/post-receive
```

If `/path/where/to/copy/files/to` is a (*non-bare*) repository itself, then:

``` sh
#!/bin/bash

GIT_WORK_TREE=/path/where/to/copy/files/to GIT_DIR=/path/where/to/copy/files/to/.git git pull origin master
exit
```
