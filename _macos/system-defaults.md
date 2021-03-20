## System defaults

<!-- MarkdownTOC -->

- [Hide Spotlight tray-icon](#hide-spotlight-tray-icon)
- [Disable send and reply animations in Mail.app](#disable-send-and-reply-animations-in-mailapp)
- [Copy just email addresses in Mail.app](#copy-just-email-addresses-in-mailapp)
- [Safari](#safari)
    - [Don’t send search queries to Apple](#don%E2%80%99t-send-search-queries-to-apple)
    - [Don't open files after download](#dont-open-files-after-download)
- [Dock](#dock)
    - [Disable the auto-hiding delay](#disable-the-auto-hiding-delay)
    - [Disable animation when hiding/showing](#disable-animation-when-hidingshowing)
    - [Make hidden applications icons translucent](#make-hidden-applications-icons-translucent)
- [Finder](#finder)
    - [Disable animations](#disable-animations)
    - [Disable warning when changing a file extension](#disable-warning-when-changing-a-file-extension)
    - [Don't create .DS_Store files on network shares and USB](#dont-create-ds_store-files-on-network-shares-and-usb)
    - [Expand save panel by default](#expand-save-panel-by-default)
    - [Disable the open application security dialog](#disable-the-open-application-security-dialog)

<!-- /MarkdownTOC -->

### Hide Spotlight tray-icon

Also hides subsequent helper. Likely requires disabling SIP.

```
$ sudo chmod 600 /System/Library/CoreServices/Search.bundle/Contents/MacOS/Search
```

### Disable send and reply animations in Mail.app

```
$ defaults write com.apple.mail DisableReplyAnimations -bool true
$ defaults write com.apple.mail DisableSendAnimations -bool true
```

### Copy just email addresses in Mail.app

So it's `thomas.anderson@metacortex.com`, not `Thomas Anderson <thomas.anderson@metacortex.com>`:

```
$ defaults write com.apple.mail AddressesIncludeNameOnPasteboard -bool false
```

### Safari

#### Don’t send search queries to Apple

```
$ defaults write com.apple.Safari UniversalSearchEnabled -bool false
$ defaults write com.apple.Safari SuppressSearchSuggestions -bool true
```

#### Don't open files after download

```
$ defaults write com.apple.Safari AutoOpenSafeDownloads -bool false
```

### Dock

#### Disable the auto-hiding delay

```
$ defaults write com.apple.dock autohide-delay -float 0
```

#### Disable animation when hiding/showing

```
$ defaults write com.apple.dock autohide-time-modifier -float 0
```

#### Make hidden applications icons translucent

```
$ defaults write com.apple.dock showhidden -bool true
```

### Finder

#### Disable animations

```
$ defaults write com.apple.finder DisableAllAnimations -bool true
```

#### Disable warning when changing a file extension

```
$ defaults write com.apple.finder FXEnableExtensionChangeWarning -bool false
```

#### Don't create .DS_Store files on network shares and USB

```
$ defaults write com.apple.desktopservices DSDontWriteNetworkStores -bool true
$ defaults write com.apple.desktopservices DSDontWriteUSBStores -bool true
```

#### Expand save panel by default

```
$ defaults write NSGlobalDomain NSNavPanelExpandedStateForSaveMode -bool true
$ defaults write NSGlobalDomain NSNavPanelExpandedStateForSaveMode2 -bool true
```

#### Disable the open application security dialog

```
$ defaults write com.apple.LaunchServices LSQuarantine -bool false
```
