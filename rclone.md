## rclone

<!-- MarkdownTOC -->

- [Listing](#listing)
- [Geting information](#geting-information)
- [Operations](#operations)
- [Excluding files](#excluding-files)
- [Remotes specifics](#remotes-specifics)
    - [AWS](#aws)

<!-- /MarkdownTOC -->

Official documentation: https://rclone.org/docs/#usage

### Listing

All files and directories:

``` sh
$ rclone lsf source:some-data
```

Only directories:

``` sh
$ rclone lsd source:some-data
```

Only files (*in the top level folder*):

``` sh
$ rclone lsl source:some-data --max-depth 1
```

### Geting information

File/folder [size](https://rclone.org/commands/rclone_size/):

``` sh
$ rclone size source:some-data/some-folder
```

[Check](https://rclone.org/commands/rclone_check/) if all the files from source are present at destination

``` sh
$ rclone check source:some-data destination:/home/data/some-data --size-only --one-way -P
```

### Operations

[Copy](https://rclone.org/commands/rclone_copy/) a file:

``` sh
$ rclone copy source:some-data/some-folder/some-file.tar destination:/home/data/some-data/some-folder -P
```

Copy a folder:

``` sh
$ rclone copy source:some-data/another-folder destination:/home/data/some-data -P --create-empty-src-dirs
```

If you are copying from local disk to local disk, especially if it is an external disk, reduce the number of simultaneous transfers (default is 4) by `--transfers 2`.

[Sync](https://rclone.org/commands/rclone_sync/) folders:

``` sh
$ rclone sync source:some-data/some-folder destination:/home/data/some-data/some-folder -P --create-empty-src-dirs --dry-run
```

When you are sure that everything is okay, run it without `--dry-run`.

### Excluding files

For one operation:

``` sh
--exclude .DS_Store
```

Globally:

``` sh
$ nano ~/.config/rclone/excludes.txt
```
``` txt
.DS_Store
```
``` sh
$ nano ~/.bashrc
```
``` sh
# rclone
RCLONE_EXCLUDE_FROM="~/.config/rclone/excludes.txt"
```

### Remotes specifics

#### AWS

With [AWS S3](https://aws.amazon.com/s3/) you must provide a bucket name, otherwise it will give you `AccessDenied` error:

``` sh
$ rclone lsf source:some-data
``` 
