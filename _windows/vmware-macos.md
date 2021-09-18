## Mac OS in VMWare on Windows

<https://www.wikigain.com/install-macos-catalina-on-vmware-on-windows/>

Get Mac OS installer and [convert it to ISO](https://github.com/retifrav/scraps/blob/master/_macos/macos.md#mac-os-installer).

Get [VMware Workstation Player](https://www.vmware.com/no/products/workstation-player/workstation-player-evaluation.html).

Get [Auto-Unlocker](https://github.com/paolo-projects/auto-unlocker) tool - it will enable creating Mac OS virtual machines in VMWare. Make sure that VMWare is completely shutdown before running the tool (*stop service just in case too*). Run the tool as Administrator and grant it network access to download required things.

Launch VMWare and create a new virtual machine. Give it 8 GB RAM, 2 cores and 100 GB disk. Set the path to ISO file that you created from Mac OS installer.

Run the virtual machine. Launch Terminal and disable SIP, if you think you might ever need to disable it, because it will be hard if not impossible to do so later:

``` sh
$ csrutil disable
```

Launch Disk Utility and erase `VMware Virtual SATA Hard Drive Media` to APFS case-sensitive.

Install the Mac OS.
