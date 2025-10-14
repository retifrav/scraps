## Git

Manual that you will never read: https://git-scm.com/book/en/

<!-- MarkdownTOC -->

- [Settings](#settings)
    - [Different user identity for different paths](#different-user-identity-for-different-paths)
- [Cloning](#cloning)
    - [Shallow clone](#shallow-clone)
    - [Single branch](#single-branch)
- [Commits](#commits)
    - [Status](#status)
    - [Tracking/staging](#trackingstaging)
        - [Stage certain operation](#stage-certain-operation)
        - [Tracking executables](#tracking-executables)
    - [Committing](#committing)
    - [Safeguarding with hooks](#safeguarding-with-hooks)
- [History](#history)
    - [Log](#log)
    - [History of a particular file](#history-of-a-particular-file)
    - [History of a specific range of lines](#history-of-a-specific-range-of-lines)
    - [Inspect a single commit](#inspect-a-single-commit)
    - [Get the date of the commit](#get-the-date-of-the-commit)
    - [Get file contents from a certain commit](#get-file-contents-from-a-certain-commit)
    - [List authors sorted by number of commits](#list-authors-sorted-by-number-of-commits)
    - [Plot author contributions per year](#plot-author-contributions-per-year)
    - [Change the author of past commits](#change-the-author-of-past-commits)
    - [Get author or committer e-mails](#get-author-or-committer-e-mails)
    - [List commits with count number](#list-commits-with-count-number)
    - [Check if particular file has been modified between commits](#check-if-particular-file-has-been-modified-between-commits)
- [Checkout or reset](#checkout-or-reset)
    - [Discard local changes](#discard-local-changes)
    - [Checkout specific commit](#checkout-specific-commit)
    - [Delete untracked](#delete-untracked)
- [Deleting commits](#deleting-commits)
    - [Reset repository history](#reset-repository-history)
    - [Remove history beyond certain commit](#remove-history-beyond-certain-commit)
    - [Delete last commits](#delete-last-commits)
        - [Deleting commits via hard reset](#deleting-commits-via-hard-reset)
        - [Deleting commits via rebase](#deleting-commits-via-rebase)
- [Branches](#branches)
    - [List branches](#list-branches)
    - [Switch to some branch](#switch-to-some-branch)
    - [Delete a branch](#delete-a-branch)
    - [Delete all branches](#delete-all-branches)
    - [Rename main to master on GitHub](#rename-main-to-master-on-github)
- [Remotes](#remotes)
- [Submodules](#submodules)
    - [Remove submodule](#remove-submodule)
- [Stash](#stash)
    - [Add current uncommitted changes to stash](#add-current-uncommitted-changes-to-stash)
    - [Apply the latest stash](#apply-the-latest-stash)
    - [List existing stashes](#list-existing-stashes)
    - [View stash](#view-stash)
    - [Delete all stashes](#delete-all-stashes)
- [Set identity and PGP key for signing commits](#set-identity-and-pgp-key-for-signing-commits)
    - [Cache PGP key password](#cache-pgp-key-password)
    - [Error gpg failed to sign the data](#error-gpg-failed-to-sign-the-data)
- [GitHub via SSH](#github-via-ssh)
- [Tags](#tags)
    - [List tags](#list-tags)
    - [Check if commit has tags](#check-if-commit-has-tags)
    - [List all the tags at specific commit](#list-all-the-tags-at-specific-commit)
    - [Get any last tag down the current branch](#get-any-last-tag-down-the-current-branch)
    - [Absolutely the last tag across all the branches](#absolutely-the-last-tag-across-all-the-branches)
    - [Delete all tags](#delete-all-tags)
- [Patches](#patches)
- [Diff the files that are not part of repository](#diff-the-files-that-are-not-part-of-repository)
- [Bare repository](#bare-repository)
    - [Create and clone](#create-and-clone)
    - [Copy files with a hook](#copy-files-with-a-hook)
- [Count the number of commits between branches](#count-the-number-of-commits-between-branches)
- [Hash of a subfolder](#hash-of-a-subfolder)

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

#### Single branch

``` sh
$ git clone --single-branch --branch some git@your.git.host:your/repository.git
```

Can be a shallow clone as well:

``` sh
$ git clone --depth 1 --single-branch --branch some git@your.git.host:your/repository.git
```

### Commits

#### Status

Status of the current repository. Shows untracked files, uncommitted changes, current branch and commit, etc:

``` bash
git status
git status -s # short version
```

#### Tracking/staging

If some files are not tracked - your commits will not include them. Start to track all the files (excluding those listed in `.gitignore`):

``` sh
$ git add .
```

Or you can track only specific files/folders:

``` sh
$ git add some/folder
```

If files are already tracked, that same command will also stage all the changes that you have. To unstage the changes:

``` sh
$ git reset -- .
```

And if you want to remove a file or folder from tracking:

``` sh
$ git rm --cached some.cpp
$ git rm -r --cached some/folder
```

##### Stage certain operation

For example, only deletions (`--deleted`) or modifications (`--modified`).

In the entire repository:

``` sh
$ git ls-files --deleted | xargs git add
```

Only in some folder:

``` sh
$ git ls-files --deleted -- some/folder | xargs git add
```

##### Tracking executables

Git doesn't care about file permissions, but it does preserve the executable attribute. So if you need to track/stage an executable:

``` sh
$ git add ./some/tool

$ git ls-files --stage ./some/tool
100644 FILE-HASH-HERE 0       some/tool

$ git update-index --chmod=+x ./some/tool

$ git ls-files --stage ./some/tool
100755 FILE-HASH-HERE 0       some/tool
```

And if you want to check the executable attribute of a file that is already committed, then:

``` sh
$ git ls-tree HEAD ./some.sh
100755 blob a747adbcc8ce9062ea8f3bf6d41c2829f7e7c2da    some.sh
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

#### Safeguarding with hooks

For instance, if you don't want to commit something that has your day-job company's name into your personal repository, you can safeguard your commits with `pre-commit` hook:

``` sh
# original hook contents
# ...

# here's where you check
forbiddenLines=$(grep -irn . --exclude-dir ".git" -e "e-corp" | wc -l)
if [ "$forbiddenLines" -ne 0 ]; then
    echo "Some files contain forbidden lines!"
    exit 1
fi

# anything after this line does not get executed for some reason
# (that's also from original hook contents)
exec git diff-index --check --cached $against --
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

#### History of a specific range of lines

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

#### Get file contents from a certain commit

The path to [file](https://github.com/retifrav/vcpkg-registry/blob/54296d0d7403a4bf3014d316a004f1c7d458001c/ports/zstd/vcpkg.json) needs to be from the top level of the repository:

``` sh
$ git show 54296d0d7403a4bf3014d316a004f1c7d458001c:ports/zstd/vcpkg.json
```

That will print the file contents into `stdout`, so you can redirect the output to save it to file somewhere (*preferably outside the current repository folder*).

Unlike checking out or restoring, `git show` works even in [bare repositories](#bare-repository).

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

#### Change the author of past commits

Just the last one:

``` sh
$ git commit --amend --author="retif <retif@decovar.dev>"
```

and probably re-sigh with a different PGP key too:

``` sh
$ git commit --amend --no-edit -S069CB68B
```

...where `069CB68B` is the key ID. And it is important that there should be no space after `-S`.

In all the commits:

``` bash
$ git filter-branch --env-filter '
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

#### Get author or committer e-mails

Author:

``` sh
$ git show -s --format="%ae" 848fbccf808503a96d67361843d231d31ab09af0
```

committer:

``` sh
$ git show -s --format="%ce" 848fbccf808503a96d67361843d231d31ab09af0
```

or both:

``` sh
$ git show -s --format="Author: %ae | Committer: %ce" 848fbccf808503a96d67361843d231d31ab09af0
```

If you want to get mapped e-mails from [.mailmap](https://git-scm.com/docs/gitmailmap) instead of the actual e-mails, then replace lower-cased `e` with upper-cased `E`, so instead of `%ae`/`%ce` it would be `%aE`/`%cE`.

#### List commits with count number

``` sh
$ git log --oneline | nl
```

#### Check if particular file has been modified between commits

If we want to check `vcpkg-configuration.json` file in the repository root:

``` sh
$ git diff-tree COMMIT-HASH-OLD..COMMIT-HASH-NEW --name-only -r -- ./vcpkg-configuration.json
```

Even though `vcpkg-configuration.json` is on top level of repository, it still wouldn't hurt to have the `-r` option too, so the lines would expand the the full repository paths, because later we'll probably need to check the repository files deeper than the top level.

The check can also be used in Bash scripts like this:

``` bash
#!/bin/bash

if [[ $(git diff-tree COMMIT-HASH-OLD..COMMIT-HASH-NEW --name-only -r -- ./vcpkg-configuration.json | wc -l) -gt 0 ]]; then
    echo 'The vcpkg-configuration.json file has been modified'
else
    echo 'No modifications in vcpkg-configuration.json'
fi
```

### Checkout or reset

#### Discard local changes

All the files:

``` sh
$ git checkout -- .
```

Just one particular file:

``` sh
$ git checkout -- some-file.py
```

#### Checkout specific commit

``` sh
$ git checkout COMMIT-HASH
```

When it fails, you can fuck everything and just switch to the bloody commit no matter what:

``` sh
$ git reset --hard COMMIT-HASH
```

here the `COMMIT-HASH` can also be a tag. Or the last 3 commits:

``` sh
$ git reset --hard HEAD~3
```

And just in case stash the changes which you don't want to lose.

Also in submodules:

``` sh
$ git submodule foreach --recursive git reset --hard
```

#### Delete untracked

What will be deleted:

``` sh
$ git clean -d -n
```

Delete untracked files and folders:

``` sh
$ git clean -d -f
```

or maybe also with `-x` to remove ignored files too:

``` sh
$ git clean -x -f -d
```

In submodules:

``` sh
$ git submodule foreach --recursive git clean -x -f -d
```

### Deleting commits

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

#### Remove history beyond certain commit

``` sh
$ git checkout --orphan temp SOME-COMMIT-HASH
$ git commit -m "Truncated history"
$ git rebase --onto temp SOME-COMMIT-HASH master
$ git branch -D temp
```

and force-push to remote.

If you'll also remove [branches](#remove-all-branches)/[tags](#remove-all-tags), do some cleanup and garbage collection:

``` sh
$ git prune --progress # delete all the objects without references
$ git gc --aggressive
$ git gc --prune=now
```

#### Delete last commits

##### Deleting commits via hard reset

``` sh
$ git log -n5
2b03831 23 minutes ago retif   (HEAD -> master, origin/master, origin/HEAD) Even more
d03f9fd 27 minutes ago retif   Another
997e3e9 36 minutes ago retif   New picture
3df1058 4 months ago retif   Книга: Страна багровых туч
dbf2e51 4 months ago retif   Deleted index file from Gollum times

$ git reset --hard HEAD~3
HEAD is now at 3df1058 Книга: Страна багровых туч

$ git log -n5
3df1058 4 months ago retif   (HEAD -> master) Книга: Страна багровых туч
dbf2e51 4 months ago retif   Deleted index file from Gollum times
039e0ad 4 months ago retif   База данных
c4e4fca 4 months ago retif   Общая информация
6844f74 4 months ago retif   Building static Qt

$ git push --force origin master
```

##### Deleting commits via rebase

Check what you have:

``` sh
$ git log --oneline --abbrev-commit

37su948 Some stupid change you want to delete
l4h5cs3 Another change
148gd35 Some change
dm36c8g First commit
```

You want to delete the latest commit (`37su948`). First change the HEAD:

``` sh
$ git rebase -i HEAD~2
```

That will get you 2 commits back. Or you can specify the commit hash:

``` sh
$ git rebase -i 24a113b
```

The text editor will open. Now simply delete the line with this commit (`37su948 Some stupid change you want to delete`), save the changes and close the editor.

If you have pushed your changes containing this commit, push this new history with force:

``` sh
$ git push --force origin master
```

On some Git hostings you might need to unprotect the branch first before force-pushing that (*and protect it back afterwards*).

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

#### Delete a branch

Local:

```
$ git branch -d test
```

Remote:

```
$ git push origin --delete test
```

#### Delete all branches

Delete all remote branches except `master`:

``` sh
$ git branch -r | grep 'origin' | grep -v 'master$' | grep -v HEAD | cut -d/ -f2- | while read line; do git push origin :heads/$line; done;
```

Also perhaps remove all local branched except the currently checkout out one:

``` sh
$ git branch --merged | grep -v \* | xargs git branch -D
```

#### Rename main to master on GitHub

[Credits](https://www.git-tower.com/learn/git/faq/git-rename-master-to-main/) to SJW-challenged Tower team.

In your local repository:

``` sh
$ git branch -m main master
$ git push -u origin master
```

Go to <https://github.com/YOURNAME/YOUR-REPOSITORY/settings/branches> and change default branch to `master`. Then:

``` sh
$ git push origin --delete main
$ git remote set-head origin master
```

Also go to <https://github.com/settings/repositories> and change default branch for new repositories back to `master`.

### Remotes

List all remotes:

``` sh
$ git remote -v
```

Check if there is anything new on any of remotes:

``` sh
$ git remote update
```

Also:

``` sh
$ git fetch --all
```

Or just the current branch:

``` sh
$ git fetch
```

Check how far you are behind:

``` sh
$ git status
```

Download commits from the current remote:

``` sh
$ git pull
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

``` sh
$ git stash
```

#### Apply the latest stash

``` sh
$ git stash pop
```

That will also delete this stash.

#### List existing stashes

``` sh
$ git stash list
```

#### View stash

Everything in the latest stash:

``` sh
$ git stash show
```

Particular file in the latest stash:

``` sh
$ git diff stash some.txt
```

#### Delete all stashes

``` sh
$ git stash clear
```

### Set identity and PGP key for signing commits

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

#### Cache PGP key password

Edit `~/.gnupg/gpg-agent.conf`:

```
# number of seconds the passphrase is cached after each invocation of GnuPG
default-cache-ttl 1111
# time after the passphrase was initially entered at which the cache is wiped
max-cache-ttl 11111
maximum-cache-ttl 11111
```

Reload or actually launch `gpg-agent`:

``` sh
$ /d/programs/GnuPG/bin/gpg-connect-agent reloadagent /bye
$ /d/programs/GnuPG/bin/gpgconf --kill gpg-agent
$ /d/programs/GnuPG/bin/gpg-agent --daemon
```

It should keep running, blocking the prompt. If it executes and returns back to prompt, then it didn't actually launch. Also might be important to launch it exactly as full path rather than just `gpg-agent --daemon` from that folder or via symlink (*sounds stupid, but that's how it was in my case*).

#### Error gpg failed to sign the data

You might get the following error:

``` sh
$ git commit
error: gpg failed to sign the data
fatal: failed to write commit object
```

To get more details:

``` sh
$ GIT_TRACE=1 git commit
21:46:21.368457 git.c:460               trace: built-in: git commit --amend --no-edit
21:46:21.387941 run-command.c:655       trace: run_command: /usr/local/bin/gpg --status-fd=2 -bsau 764367E8069CB68B
error: gpg failed to sign the data
fatal: failed to write commit object
```

Just in case, verify that `764367E8069CB68B` is your PGP key ID. But in general these details don't help much either, however you can now run the failing command directly:

``` sh
$ /usr/local/bin/gpg --status-fd=2 -bsau 764367E8069CB68B
[GNUPG:] KEY_CONSIDERED 6082FCD476D10010CF47F699764367E8069CB68B 2
[GNUPG:] BEGIN_SIGNING H8
```

and it just stucks there, so you can only interrupt it with `Ctrl + C`. There are several workarounds/solutions for resolving this ([this one with comments](https://stackoverflow.com/a/40066889/1688203) or [this one](https://stackoverflow.com/a/58930186/1688203)). Exporting `GPG_TTY=$(tty)` fixes it for committing via command line - you will start getting TUI dialogs for entering the key password, but to bring back the GUI prompts you'll need to install a different version of `pinentry` and restart the `gpg-agent`:

``` sh
$ brew install pinentry-mac
$ killall gpg-agent && gpg-agent --daemon --pinentry-program /usr/local/bin/pinentry-mac
```

If it still fails, check that you have `~/.gnupg/gpg-agent.conf` with some content like:

``` ini
default-cache-ttl 600
max-cache-ttl 7200
```

But even that might not be enough, so also do this (*which apparently is also required after any modifications to the `gpg-agent.conf`*):

``` sh
$ gpgconf --kill gpg-agent
```

### GitHub via SSH

Generate an [SSH-key](/_linux/ssh.md#generate-a-new-ssh-key):

``` bash
$ cd ~/.ssh/
$ ssh-keygen -t ed25519 -a 100 -C "your.email@example.com" -b 4096
```

Enter the file name, for example `id_rsa_github_yourname`. Two keys will be generated:

* public: `id_rsa_github_yourname.pub`;
* private: `id_rsa_github_yourname`.

Change permissions just in case:

``` bash
$ chmod 400 ~/.ssh/id_rsa_github_yourname*
```

Copy contents of `id_rsa_github_yourname.pub` and save it to your [GitHub account](https://github.com/settings/keys).

Now add the key to keychain and test it:

``` bash
$ ssh-add ~/.ssh/id_rsa_github_yourname
$ ssh-add -l
$ ssh -T git@github.com
```

Or add a new record in `~/.ssh/config`.

Also don't forget to add remote repository using its SSH link and not HTTP. For example: `ssh://git@github.com:retifrav/scraps.git`

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
$ git describe --exact-match COMMIT-HASH-HERE
```

or

```
$ git describe --exact-match --abbrev=0
```

#### List all the tags at specific commit

``` sh
$ git tag --points-at COMMIT-HASH-HERE
```

#### Get any last tag down the current branch

```
$ git describe --tags --abbrev=0 --match "build-*"
```

#### Absolutely the last tag across all the branches

```
$ git describe --tags $(git rev-list --tags --max-count=1)
```

#### Delete all tags

From remote:

``` sh
$ git tag | xargs -L 1 | xargs git push origin --delete
```

It might fail to delete some, then:

``` sh
$ git tag -l | xargs -n 1 git push --delete origin
```

If this fails too, then previous command might have messed something, re-clone the repository and run that last command again. It will take forever to finish, but it will do the job.

Now local tags:

``` sh
$ git tag | xargs -L 1 | xargs git tag --delete
```

And again that other variant, if some fail:

``` sh
$ git tag -l | xargs -n 1 git tag --delete
```

### Patches

If you want to create a patch from your uncommitted changes (*including non-text files such as images*), **stage** everything (*what you want to include to the patch*) first and then from your repository root:

``` sh
$ git diff --cached --binary > some.patch
```

Or you can do that without staging, then you'll just need to list those files (*or/and folders*):

``` sh
$ git diff --binary \
    some/file.txt \
    another.py \
    different.py \
    some-folder \
    config/settings.ini \
    thing.cpp \
    > some.patch
```

To apply this patch:

``` sh
$ git apply ~/Downloads/some.patch
```

### Diff the files that are not part of repository

``` sh
$ git diff --no-index ./qtbase/mkspecs/common/mac.conf ~/Desktop/mac.conf
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

### Count the number of commits between branches

For example in [PDFium](https://pdfium.googlesource.com/pdfium) repository:

``` sh
$ echo "$(($(git rev-list --count chromium/6656) - $(git rev-list --count chromium/5304)))"
2439
```

### Hash of a subfolder

When it is already committed:

``` sh
$ git rev-parse HEAD:ports/some
686f602df6f26bc3b5ee212657fa2a1b8fee2f1e
```

or:

``` sh
$ git ls-tree HEAD ./ports/some | cut -d' ' -f3 | cut -f1
686f602df6f26bc3b5ee212657fa2a1b8fee2f1e
```

And if changes are not committed yet, then:

``` sh
$ git add ./ports/some
$ git write-tree --prefix=ports/some
686f602df6f26bc3b5ee212657fa2a1b8fee2f1e
```

You might want/need to do this in a [temporary index](https://stackoverflow.com/a/48213033/1688203):

``` sh
$ cp .git/index /tmp/git_index
$ export GIT_INDEX_FILE=/tmp/git_index
$ git add ./ports/some
$ git write-tree --prefix=ports/some
686f602df6f26bc3b5ee212657fa2a1b8fee2f1e
$ unset GIT_INDEX_FILE
```

It can be then verified with:

``` sh
$ git cat-file -t 686f602df6f26bc3b5ee212657fa2a1b8fee2f1e
tree

$ git cat-file -p 686f602df6f26bc3b5ee212657fa2a1b8fee2f1e
100644 blob 3698da3c118791dd3d644b40c525c7c3aad330a5    001-dynamic-library.patch
100644 blob 3c1254caadd649f89889939894beede7deaef758    002-installation.patch
100644 blob 929abf2cb37c3916ddac7d86c6d52067147167e1    portfile.cmake
100644 blob 343d30a6ca6151098b890c71bba1f47bf8b0eb27    vcpkg.json
```
