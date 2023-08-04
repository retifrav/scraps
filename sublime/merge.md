## Sublime Merge

<!-- MarkdownTOC -->

- [Extending Command Palette using Git aliases](#extending-command-palette-using-git-aliases)

<!-- /MarkdownTOC -->

### Extending Command Palette using Git aliases

One can have [aliases](https://git-scm.com/book/en/v2/Git-Basics-Git-Aliases) - short commands that expand to much longer ones. This functionality is very useful for extending available commands in Sublime Merge.

For example, here's an alias for making a patch from staged files:

``` sh
$ nano ~/.gitconfig

[alias]
    mkpatch = "!git diff --cached --binary > ./some.patch"
```

And then in `Default.sublime-commands`:

``` json
{
    "caption": "Create a patch from staged changes",
    "command": "git",
    "args":
    {
        "argv": ["mkpatch"]
    }
}
```

Now you can call it from Command Palette.
