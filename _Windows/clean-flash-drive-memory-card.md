Start `cmd.exe` with administrative rights. Run:

``` cmd
diskpart
```

**Diskpart** utility will launch. Execute:

``` cmd
list disk
```

Choose the disk (pay attention to the numbers in **your** list):

``` cmd
select disk 2
```

Now clean the selected disk:

``` cmd
clean
```

And create a new partition:

``` cmd
create partition primary
```

Now select it:

``` cmd
select partition 1
```

Make it active:

``` cmd
active
```

Format it (replace `NTFS` with `FAT32`, if you wish):

``` cmd
format fs=NTFS quick
```

And finally assign a letter:

``` cmd
assign letter=Z
```

You can exit **diskpart** now:

``` cmd
exit
```

## Windows XP note

**Diskpart** won't see you disk, so you have to install **Hitachi Microdrive** drivers. It also doesn't have the `format` command, so you need to do it with a standard formatter after assigning a letter to the disk (exit **diskpart** and format the disk).