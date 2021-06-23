## Mounting Clonezilla backup image

So you have a CLonezilla backup image (*in my case Windows*) on an external disk and you would like to extract some files from it.

Let's take a look at the backup contents:

```
$ ls -L1 /media/backups/images/2019-11-16-img/
Info-dmi.txt
Info-img-id.txt
Info-lshw.txt
Info-lspci.txt
Info-packages.txt
Info-saved-by-cmd.txt
blkdev.list
blkid.list
clonezilla-img
dev-fs.list
parts
sda-chs.sf
sda-hidden-data-after-mbr
sda-mbr
sda-pt.parted
sda-pt.parted.compact
sda-pt.sf
sda1.info
sda1.ntfs-ptcl-img.gz.aa
sda2.ntfs-ptcl-img.gz.aa
sda2.ntfs-ptcl-img.gz.ab
sda2.ntfs-ptcl-img.gz.ac
sda2.ntfs-ptcl-img.gz.ad
sda2.ntfs-ptcl-img.gz.ae
sda2.ntfs-ptcl-img.gz.af
sda3.ext2-ptcl-img.gz.aa
sda6.ext4-ptcl-img.gz.aa
sda6.ext4-ptcl-img.gz.ab
sdb-chs.sf
sdb-hidden-data-after-mbr
sdb-mbr
sdb-pt.parted
sdb-pt.parted.compact
sdb-pt.sf
sdb1.ntfs-ptcl-img.gz.aa
sdb1.ntfs-ptcl-img.gz.ab
sdb1.ntfs-ptcl-img.gz.ac
sdb1.ntfs-ptcl-img.gz.ad
sdb1.ntfs-ptcl-img.gz.ae
sdb1.ntfs-ptcl-img.gz.af
sdb1.ntfs-ptcl-img.gz.ag
sdb1.ntfs-ptcl-img.gz.ah
sdb1.ntfs-ptcl-img.gz.ai
sdb1.ntfs-ptcl-img.gz.aj
sdb1.ntfs-ptcl-img.gz.ak
sdb1.ntfs-ptcl-img.gz.al
sdb1.ntfs-ptcl-img.gz.am
sdb1.ntfs-ptcl-img.gz.an
sdb1.ntfs-ptcl-img.gz.ao
sdb1.ntfs-ptcl-img.gz.ap
sdb1.ntfs-ptcl-img.gz.aq
sdb1.ntfs-ptcl-img.gz.ar
sdb1.ntfs-ptcl-img.gz.as
sdb1.ntfs-ptcl-img.gz.at
sdb1.ntfs-ptcl-img.gz.au
sdb1.ntfs-ptcl-img.gz.av
sdb1.ntfs-ptcl-img.gz.aw
sdb1.ntfs-ptcl-img.gz.ax
sdb1.ntfs-ptcl-img.gz.ay
sdb1.ntfs-ptcl-img.gz.az
sdb1.ntfs-ptcl-img.gz.ba
sdb1.ntfs-ptcl-img.gz.bb
sdb1.ntfs-ptcl-img.gz.bc
sdb1.ntfs-ptcl-img.gz.bd
sdb1.ntfs-ptcl-img.gz.be
```

I have several disks and partitions here, and I need to get files from disk `C` (*and I know that it's in these files: `sda2.ntfs-ptcl-img.gz.*`*). So what I need to do is to concat an image archive from its splitted parts, unpack it to an image file and mount it using `partclone`, which I need to install first:

```
$ sudo apt install partclone
```

First I tried [this method](https://serverfault.com/a/401797/410288):

```
$ zcat /media/backups/images/2019-11-16-img/sda2.ntfs-ptcl-img.gz.* | sudo partclone.restore --restore_raw_file -C -s - -o ~/Downloads/c.img
```

But that failed with:

```
gzip: /media/backups/images/2019-11-16-img/sdb1.ntfs-ptcl-img.gz.aa: unexpected end of file
read ERROR:Success
Partclone fail, please check /var/log/partclone.log
```

So I figured that perhaps my image is not compressed or compressed in some other way, so I did this instead:

```
$ cat /media/backups/images/2019-11-16-img/sda2.ntfs-ptcl-img.gz.* > ~/Downloads/c-img.gz
```

And then:

```
$ cd ~/Downloads
$ gunzip -k c-img.gz
```

And then:

```
$ sudo partclone.restore --restore_raw_file -C -s c-img -o ~/Downloads/c.img
```

Or [all 3 in one](https://ubuntuforums.org/showthread.php?t=2007760#post_12495659):

```
$ cd ~/Downloads
$ cat /media/backups/images/2019-11-16-img/sda2.ntfs-ptcl-img.gz.* | gzip -d -c | sudo partclone.restore --restore_raw_file -C -s - -o c.img
```

And finally:

```
$ sudo mount -o loop c.img /mnt -t ntfs -o ro
```

Now I can browse the disk `C` contents in `/mnt`:

To unmount the image:

```
$ sudo umount c.img
```
