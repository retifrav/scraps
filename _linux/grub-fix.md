# Fixing GRUB unknown filesystem

Based on [this article](https://www.easytechguides.com/error-unknown-filesystem-grub-rescue.html).

So, one day you boot your PC and you get this error:

``` bash
error: unknown filesystem.
grub rescue>
```

Get the list of all the partitions:

``` bash
ls
```

This will show you all the partitions on your hard drive.

*Example 1:*
``` bash
grub rescue> ls (hd0) (hd0,gpt5) (hd0,gpt4) (hd0,gpt3) (hd0,gpt2) (hd0,gpt1) (fd0) grub rescue>
```

*Example 2:*
``` bash
grub rescue> ls (hd0) (hd0,msdos5) (hd0,msdos4) (hd0,msdos3) (hd0,msdos2) (hd0,msdos1) (fd0) grub rescue>
```

Now you need to find the partition where Linux is installed on:

``` bash
ls (hd0,PARTITION-NAME)
```

Do this replacing `PARTITION-NAME` with every partition from the list until you see something that looks like a Linux folder structure:

``` bash
grub rescue> ls (hd0,gpt5)/./ ../ lost+found/ etc/ media/ bin/ boot/ dev/ home/ lib/ mnt/ opt/ proc/ root/ run/ sbin/ selinux/ srv/ sys/ tmp/ usr/ var/ initrd.img initrd.img.old vmlinuz cdrom/ grub rescue>
```

As you can see, in my example `(hd0,gpt5)` is the correct partition.

Do this:

``` bash
set prefix=(hd0,gpt5)/boot/grub
set root=(hd0,gpt5)
```

Check if filesystem root was set correctly:

``` bash
ls /
```

Now do this:

``` bash
insmod normal
normal
```

A boot menu will appear and display the list of installed operating systems. Boot into Linux, open terminal and fix GRUB settings:

``` bash
sudo update-grub
sudo grub-install /dev/sda
```

Now it's fixed, you can reboot your PC and check.
