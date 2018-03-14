# Git

https://git-scm.com/book/en/

* [Settings](#settings)
* [Repository](#repository)
    - [Local](#local)
    - [Remote](#remote)

## Settings

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

## Repository

### Local

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

History of commits:

``` bash
git log
git log --stat # detailed history of commits. You can scroll the output and use `q` for exit
git log --pretty=oneline # formatted history of commits. Other possible values are `short`, `full` and `format`
git log --since=2.seeks # history of commits for the past 2 weeks (or any other period)
```

### Remote

List all remote repositories for the current local repository:

``` bash
git remote -v
```

Add a new remote repository and name it `SomeRepo`:

``` bash
git remote add SomeRepo https://github.com/some/repo
```

Get contents of the default remote repository (`origin/master`):

``` bash
git fetch
```

Show contents of the remote repository `SomeOtherRemote`:

``` bash
git remote show SomeOtherRemote
```

Push changes to remote:

``` bash
git push # pushes your commits from `master` branch to the default remote repository (`origin/master`)
git push SomeRepo someBranch # pushes commits from `someBranch` to `SomeRepo` remote repository
```
