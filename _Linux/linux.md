## Linux

- [Update packages](#update-packages)
- [Delete packages](#delete-packages)

### Update packages

``` bash
apt-get update
apt-get upgrade
apt-get dist-upgrade
apt-get autoremove
```

### Delete packages

For example, we want to delete **LibreOffice**. All of its packages names start with `libreoffice`, so:

``` bash
sudo apt-get remove --purge libreoffice*
sudo apt-get clean
sudo apt-get autoremove
```
