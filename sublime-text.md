## Sublime Text

<!-- MarkdownTOC -->

- [Console](#console)
    - [Current Python version](#current-python-version)
    - [Enable commands logging](#enable-commands-logging)
    - [Run a plugin command](#run-a-plugin-command)
    - [General information](#general-information)

<!-- /MarkdownTOC -->

### Console

To open/show: `View` → `Show Console`. This is a Python interactive console, so you can execute not only commands from Sublime Text plugins, but any other Python code (*bearing in mind that this is an embedded Python without your system packages installed from PyPI*).

#### Current Python version

``` py
import sys; sys.version
```

Once again, that is Sublime's embedded Python version, not your system one's.

Also, by right-clicking on input field you can switch between older and newer versions (*if your Sublime Text ships with more than one*).

#### Enable commands logging

``` py
sublime.log_commands(True)
```

Now any command you execute will be logged in the console output, so you can for example assign keybindings to them or inspect their arguments.

#### Run a plugin command

If a plugin has a command named `ExampleCommand`, you can call it from console:

``` py
view.run_command("example")
```

#### General information

Available at any time:

``` py
sublime.version()
sublime.channel()
sublime.platform()
sublime.arch()
```

Available only after the `plugin_host` and plugins are loaded (*so you will need to call these from `plugin_loaded()` of your plugin*):

``` py
sublime.packages_path()
sublime.installed_packages_path()
```
