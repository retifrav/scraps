# Node.js

How to install [this piece of shit](https://nodejs.org/) from their APT repository.

<!-- MarkdownTOC -->

- [Manually](#manually)
- [Via their script](#via-their-script)

<!-- /MarkdownTOC -->

## Manually

``` sh
$ sudo apt install apt-transport-https ca-certificates curl gnupg

$ curl -fsSL https://deb.nodesource.com/gpgkey/nodesource-repo.gpg.key | sudo gpg --dearmor -o /usr/share/keyrings/nodesource.gpg

$ node_version="21.x" # or whichever you need
$ arch=$(dpkg --print-architecture)

$ echo "deb [arch=$arch signed-by=/usr/share/keyrings/nodesource.gpg] https://deb.nodesource.com/node_$node_version nodistro main" | sudo tee /etc/apt/sources.list.d/nodesource.list > /dev/null
```

``` sh
# not sure if this one is actually required
$ sudo nano /etc/apt/preferences.d/nsolid
```
``` sh
Package: nsolid
Pin: origin deb.nodesource.com
Pin-Priority: 600
```

``` sh
$ sudo nano /etc/apt/preferences.d/nodejs
```
``` sh
Package: nodejs
Pin: origin deb.nodesource.com
Pin-Priority: 600
```

``` sh
$ sudo apt update
$ sudo apt install nodejs
```

## Via their script

The [same steps](#manually), but as a [script](https://deb.nodesource.com/setup_21.x):

``` sh
#!/bin/bash

# Logger Function
log() {
  local message="$1"
  local type="$2"
  local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
  local color
  local endcolor="\033[0m"

  case "$type" in
    "info") color="\033[38;5;79m" ;;
    "success") color="\033[1;32m" ;;
    "error") color="\033[1;31m" ;;
    *) color="\033[1;34m" ;;
  esac

  echo -e "${color}${timestamp} - ${message}${endcolor}"
}

# Error handler function  
handle_error() {
  local exit_code=$1
  local error_message="$2"
  log "Error: $error_message (Exit Code: $exit_code)" "error"
  exit $exit_code
}

# Function to check for command availability
command_exists() {
  command -v "$1" &> /dev/null
}

check_os() {
    if ! [ -f "/etc/debian_version" ]; then
        echo "Error: This script is only supported on Debian-based systems."
        exit 1
    fi
}

# Function to Install the script pre-requisites
install_pre_reqs() {
    log "Installing pre-requisites" "info"

    # Run 'apt-get update'
    if ! apt-get update -y; then
        handle_error "$?" "Failed to run 'apt-get update'"
    fi

    # Run 'apt-get install'
    if ! apt-get install -y apt-transport-https ca-certificates curl gnupg; then
        handle_error "$?" "Failed to install packages"
    fi

    mkdir -p /usr/share/keyrings
    rm -f /usr/share/keyrings/nodesource.gpg
    rm -f /etc/apt/sources.list.d/nodesource.list

    # Run 'curl' and 'gpg'
    if ! curl -fsSL https://deb.nodesource.com/gpgkey/nodesource-repo.gpg.key | gpg --dearmor -o /usr/share/keyrings/nodesource.gpg; then
      handle_error "$?" "Failed to download and import the NodeSource signing key"
    fi
}

# Function to configure the Repo
configure_repo() {
    local node_version=$1

    arch=$(dpkg --print-architecture)
    if [ "$arch" != "amd64" ] && [ "$arch" != "arm64" ] && [ "$arch" != "armhf" ]; then
      handle_error "1" "Unsupported architecture: $arch. Only amd64, arm64, and armhf are supported."
    fi

    echo "deb [arch=$arch signed-by=/usr/share/keyrings/nodesource.gpg] https://deb.nodesource.com/node_$node_version nodistro main" | tee /etc/apt/sources.list.d/nodesource.list > /dev/null

    # N|solid Config
    echo "Package: nsolid" | tee /etc/apt/preferences.d/nsolid > /dev/null
    echo "Pin: origin deb.nodesource.com" | tee -a /etc/apt/preferences.d/nsolid > /dev/null
    echo "Pin-Priority: 600" | tee -a /etc/apt/preferences.d/nsolid > /dev/null

    # Nodejs Config
    echo "Package: nodejs" | tee /etc/apt/preferences.d/nodejs > /dev/null
    echo "Pin: origin deb.nodesource.com" | tee -a /etc/apt/preferences.d/nodejs > /dev/null
    echo "Pin-Priority: 600" | tee -a /etc/apt/preferences.d/nodejs > /dev/null

    # Run 'apt-get update'
    if ! apt-get update -y; then
        handle_error "$?" "Failed to run 'apt-get update'"
    else
        log "Repository configured successfully. To install Node.js, run: apt-get install nodejs -y" "success"
    fi
}

# Define Node.js version
NODE_VERSION="21.x"

# Check OS
check_os

# Main execution
install_pre_reqs || handle_error $? "Failed installing pre-requisites"
configure_repo "$NODE_VERSION" || handle_error $? "Failed configuring repository"
```
