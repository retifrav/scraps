## Python

- [Web server](#web-server)
- [Dictionary as switch](#dictionary-as-switch)
- [Colors in print](#colors-in-print)

### Web server

Run in a folder to serve static HTML pages:

```
python -m http.server 8000
```

### Dictionary as switch

```
statusCodesColors = {
    200: "green",
    301: "yellow",
    302: "yellow",
    401: "yellow",
    403: "yellow"
}


def getColorForStatus(statusCode):
    print(statusCodesColors.get(statusCode, "red"))


getColorForStatus(302)
```

### Colors in print

```
pip install colorama
pip install termcolor
```

**Colorama** is needed for **termcolor** to work in Windows console (`cmd.exe`).

```
import colorama
from termcolor import colored

colorama.init()

print(colored("All good", "green"))
```
