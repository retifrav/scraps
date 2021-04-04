## Azure CLI

<!-- MarkdownTOC -->

- [Installation](#installation)
    - [Mac OS](#mac-os)
- [Adding and deleting users](#adding-and-deleting-users)
- [Fixing sudoers](#fixing-sudoers)

<!-- /MarkdownTOC -->

### Installation

<https://docs.microsoft.com/en-us/cli/azure/install-azure-cli>

#### Mac OS

``` sh
$ brew install azure-cli
$ az login
```

### Adding and deleting users

If you messed up your current user, you can create a new one - it will have rights to run `sudo`, so you'll be able to fix stuff with it:

``` sh
$ az vm user update \
--resource-group YOUR-RESOURCE-GROUP \
--name YOUR-VM-NAME \
--username petya \
--ssh-key-value ~/.ssh/YOUR-VM-NAME.pub
```

When you are done fixing stuff, you can delete that user:

``` sh
$ az vm user delete \
  --resource-group YOUR-RESOURCE-GROUP \
  --name YOUR-VM-NAME \
  --username petya
```

### Fixing sudoers

If you messed up your `/etc/sudoers.d/whatever` and getting errors like:

``` sh
>>> /etc/sudoers.d/90-cloud-init-users: syntax error near line 5 <<<
sudo: parse error in /etc/sudoers.d/90-cloud-init-users near line 5
sudo: no valid sudoers sources found, quitting
sudo: unable to initialize policy plugin
```

Lower the permissions to the `sudoers.d` and its contents, so you could edit that file:

``` sh
$ az vm run-command invoke \
-g YOUR-RESOURCE-GROUP -n YOUR-VM-NAME \
--command-id RunShellScript \
--scripts "chmod 777 /etc/sudoers.d"

$ az vm run-command invoke \
-g YOUR-RESOURCE-GROUP -n YOUR-VM-NAME \
--command-id RunShellScript \
--scripts "chmod 777 /etc/sudoers.d/*"
```

Fix the error (*back on the server via SSH*):

``` sh
$ nano /etc/sudoers.d/whatever
```

Put permissions back:

``` sh
$ az vm run-command invoke \
-g YOUR-RESOURCE-GROUP -n YOUR-VM-NAME \
--command-id RunShellScript \
--scripts "chmod 440 /etc/sudoers.d/*"

$ az vm run-command invoke \
-g YOUR-RESOURCE-GROUP -n YOUR-VM-NAME \
--command-id RunShellScript \
--scripts "chmod 440 /etc/sudoers.d"
```
