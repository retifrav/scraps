## NGINX

- [Windows](#windows)

### Windows

<http://nginx.org/en/docs/windows.html>

Download the [archive](https://nginx.org/en/download.html), unpack it and open that directory (`d:\programs`) in `cmd`.

```
> cd d:\programs\nginx
> start nginx
```

Check if it's running:

```
> tasklist /fi "imagename eq nginx.exe"
```

Config is in `d:\programs\conf\nginx.conf`, default webroot is in `d:\programs\html`.

To reload the config:

```
> nginx -s reload
```

To quit NGINX gracefully (takes some time):

```
> nginx -s quit
```

To shutdown NGINX right away:

```
> nginx -s stop
```
