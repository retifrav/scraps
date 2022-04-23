## Sublime Text

<!-- MarkdownTOC -->

- [Console](#console)
    - [Current Python version](#current-python-version)
    - [Enable commands logging](#enable-commands-logging)
- [Plugins API](#plugins-api)
    - [Run a plugin command](#run-a-plugin-command)
    - [General information](#general-information)
    - [Open a file via GUI dialog](#open-a-file-via-gui-dialog)
        - [Using lambda](#using-lambda)
        - [Using functools.partial](#using-functoolspartial)
        - [Using double lambda](#using-double-lambda)

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

### Plugins API

#### Run a plugin command

If a plugin has a View command named `ExampleCommand`, you can call it from console:

``` py
view.run_command("example")
```

or if it's a Window command, then:

``` py
window.run_command("example")
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

#### Open a file via GUI dialog

The callback function is the same for all variants:

``` py
def openSomeFile(
        window: sublime.Window,
        selectedFile: str
        ) -> None:
    #print(f"Total views: {len(window.views())}")
    #print(f"Is file already open: {window.find_open_file(selectedFile)}")
    if selectedFile:
        try:
            window.open_file(
                selectedFile
            )
        except Exception as ex:
            print(f"error: {ex}")
            sublime.error_message(
                " ".join((
                    "There was an error trying to open the file.",
                    "Check console for details."
                ))
            )
            return
```

But note that only the variant with [double lambda](#using-double-lambda) provides correctly working access to `window` properties and methods, such as `views()` and `find_open_file()` (<https://github.com/sublimehq/sublime_text/issues/5382>).

##### Using lambda

``` py
class OpenSomeFileCommand(sublime_plugin.WindowCommand):
    def run(self):
        originalFile: pathlib.Path = pathlib.Path(
            self.window.active_view().file_name()
        )

        sublime.open_dialog(
            lambda f: openSomeFile(
                self.window,
                f
            ),
            [],
            str(originalFile.parents[0]),
            False,
            False
        )
```

##### Using functools.partial

``` py
class OpenSomeFileCommand(sublime_plugin.WindowCommand):
    def run(self):
        originalFile: pathlib.Path = pathlib.Path(
            self.window.active_view().file_name()
        )

        f = functools.partial(openSomeFile, self.window)

        sublime.open_dialog(
            f,
            [],
            str(originalFile.parents[0]),
            False,
            False
        )
```

##### Using double lambda

``` py
class OpenSomeFileCommand(sublime_plugin.WindowCommand):
    def run(self):
        originalFile: pathlib.Path = pathlib.Path(
            self.window.active_view().file_name()
        )

        sublime.open_dialog(
            lambda f: sublime.set_timeout(
                lambda: openSomeFile(
                    self.window,
                    f
                )
            ),
            [],
            str(originalFile.parents[0]),
            False,
            False
        )
```
