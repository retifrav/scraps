## WSL

<!-- MarkdownTOC -->

- [Version and status](#version-and-status)
- [Distributions](#distributions)
    - [Installed](#installed)
    - [Available for installation](#available-for-installation)
    - [Installation](#installation)
- [Moving distribution to a different drive](#moving-distribution-to-a-different-drive)

<!-- /MarkdownTOC -->

### Version and status

Current version:

``` sh
$ wsl --version
WSL version: 2.5.9.0
Kernel version: 6.6.87.2-1
WSLg version: 1.0.66
MSRDC version: 1.2.6074
Direct3D version: 1.611.1-81528511
DXCore version: 10.0.26100.1-240331-1435.ge-release
Windows version: 10.0.22000.2538
```

Update:

``` sh
$ wsl --update
```

Status:

``` sh
$ wsl --status
Default Distribution: Ubuntu
Default Version: 2
```

### Distributions

#### Installed

``` sh
$ wsl --list --verbose
```

#### Available for installation

``` sh
$ wsl --list --online

NAME                            FRIENDLY NAME
AlmaLinux-8                     AlmaLinux OS 8
AlmaLinux-9                     AlmaLinux OS 9
AlmaLinux-Kitten-10             AlmaLinux OS Kitten 10
AlmaLinux-10                    AlmaLinux OS 10
Debian                          Debian GNU/Linux
FedoraLinux-42                  Fedora Linux 42
SUSE-Linux-Enterprise-15-SP6    SUSE Linux Enterprise 15 SP6
SUSE-Linux-Enterprise-15-SP7    SUSE Linux Enterprise 15 SP7
Ubuntu                          Ubuntu
Ubuntu-24.04                    Ubuntu 24.04 LTS
archlinux                       Arch Linux
kali-linux                      Kali Linux Rolling
openSUSE-Tumbleweed             openSUSE Tumbleweed
openSUSE-Leap-15.6              openSUSE Leap 15.6
Ubuntu-18.04                    Ubuntu 18.04 LTS
Ubuntu-20.04                    Ubuntu 20.04 LTS
Ubuntu-22.04                    Ubuntu 22.04 LTS
OracleLinux_7_9                 Oracle Linux 7.9
OracleLinux_8_7                 Oracle Linux 8.7
OracleLinux_9_1                 Oracle Linux 9.1
```

#### Installation

``` sh
$ mkdir -p e:/wsl/ubuntu
$ wsl --install --distribution Ubuntu --location e:/wsl/ubuntu/ --name ubuntu --no-launch
```

### Moving distribution to a different drive

Via managing:

``` sh
$ wsl --shutdown

$ wsl --list --verbose
  NAME      STATE           VERSION
* ubuntu    Stopped         2

$ wsl --manage ubuntu --move d:/wsl/ubuntu

$ ls -L1 d:/wsl/
ext4.vhdx

$ du -hs d:/wsl/
12G     d:/wsl/

$ wsl
```

Via importing:

``` sh
$ wsl --shutdown

$ wsl --list --verbose
  NAME      STATE           VERSION
* ubuntu    Stopped         2

$ wsl --export ubuntu d:/temp/ubuntu.tar
$ wsl --unregister ubuntu
$ wsl --import ubuntu d:/wsl/ubuntu/ d:/temp/ubuntu.tar

$ ls -L1 d:/wsl/
ext4.vhdx

$ du -hs d:/wsl/
12G     d:/wsl/

$ wsl
```
