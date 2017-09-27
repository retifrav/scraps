# Users


## All users in the system

``` bash
cat /etc/passwd | awk -F ':' '{ print $1 }'
```

## Last logons

``` bash
lastlog
```

## Change your password

``` bash
passwd
```

## Create new user

``` bash
useradd vasya
passwd vasya
```
