## Midnight Commander

<https://midnight-commander.org/>

You can also [build it](/_building/midnight-commander.md) from sources.

<!-- MarkdownTOC -->

- [Auto-complete](#auto-complete)
- [Sync panel](#sync-panel)
- [Sorting panel by modification date](#sorting-panel-by-modification-date)

<!-- /MarkdownTOC -->

### Auto-complete

<https://unix.stackexchange.com/questions/250818/how-to-use-command-line-completion-in-midnight-commander>

```
Option/Alt + Tab
```

or

```
ESC + Tab
```

### Sync panel

<https://unix.stackexchange.com/questions/5901/how-to-open-the-same-directory-in-another-panel-in-midnight-commander>

To current path:

```
Option/Alt + I
```

To path under cursor:

```
Option/Alt + O
```

### Sorting panel by modification date

In `~/.config/mc/mc.keymap`:

``` ini
[panel]
SortByMTime = alt-m
SortByName = alt-n
```

Pressing the combination again reverses the sorting order.
