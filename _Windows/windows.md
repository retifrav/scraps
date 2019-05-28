## Windows

### Get disk allocation unit size

``` bash
$ diskpart

DISKPART> list volume

  Volume ###  Ltr  Label        Fs     Type        Size     Status     Info
  ----------  ---  -----------  -----  ----------  -------  ---------  --------
  Volume 0     D   data         NTFS   Partition   1863 GB  Healthy
  Volume 1         Recovery     NTFS   Partition    499 MB  Healthy
  Volume 2     C   system       NTFS   Partition    299 GB  Healthy    Boot
  Volume 3     E   work         NTFS   Partition    400 GB  Healthy
  Volume 4                      FAT32  Partition     99 MB  Healthy    System
  Volume 5     F   Samsung_T5   exFAT  Partition   1863 GB  Healthy

DISKPART> select volume 5

Volume 5 is the selected volume.

DISKPART> filesystem

Current File System

  Type                 : exFAT
  Allocation Unit Size : 128K
  Flags : 00000000
```
