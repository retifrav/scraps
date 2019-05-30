## rclone

Official documentation: https://rclone.org/docs/#usage

My examples are based on interaction with [AWS S3](https://aws.amazon.com/s3/) instance - there you must provide a bucket name, (`some-data`) otherwise it will give you `AccessDenied` error.

### Listing

All files and directories:

```
rclone lsf source:some-data
```

Only directories:

```
rclone lsd source:some-data
```

Only files (in the top level folder):

```
rclone lsl source:some-data --max-depth 1
```

### Geting information

File/folder [size](https://rclone.org/commands/rclone_size/):

```
rclone size source:some-data/some-folder
```

[Check](https://rclone.org/commands/rclone_check/) if all the files from source are present at destination

```
rclone check source:some-data destination:/home/data/some-data --size-only --one-way -P
```

### Operations

[Copy](https://rclone.org/commands/rclone_copy/) a file/folder:

```
rclone copy source:some-data/some-folder/some-file.tar destination:/home/data/some-data/some-folder -P
```

or

```
rclone copy source:some-data/another-folder destination:/home/data/some-data -P --create-empty-src-dirs
```

If you’re copying from local disk to local disk, especially if it is an external disk, reduce the number of simultaneous transfers (default is 4) by `--transfers 2`.

[Sync](https://rclone.org/commands/rclone_sync/) folders:

```
rclone sync source:some-data/some-folder destination:/home/data/some-data/some-folder -P --create-empty-src-dirs --dry-run
```

When you’re sure that everything is okay, run it without `--dry-run`.
