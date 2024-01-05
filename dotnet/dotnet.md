## .NET

[.NET](https://dotnet.microsoft.com/) (*former .NET Core*).

<!-- MarkdownTOC -->

- [Maintenance](#maintenance)
    - [List installed versions](#list-installed-versions)
    - [Install from Microsoft feed](#install-from-microsoft-feed)
        - [Prefer Microsoft feed over Ubuntu](#prefer-microsoft-feed-over-ubuntu)
    - [Install specific version](#install-specific-version)
    - [Delete particular version](#delete-particular-version)
- [Create a new project](#create-a-new-project)
    - [Specific SDK version](#specific-sdk-version)
    - [MVC project with authentication](#mvc-project-with-authentication)
- [NuGet packages](#nuget-packages)
- [Publish the project](#publish-the-project)

<!-- /MarkdownTOC -->

### Maintenance

#### List installed versions

``` sh
$ dotnet --info
$ dotnet --list-sdks
$ ls -L1 /usr/share/dotnet/sdk
$ dotnet --list-runtimes
```

#### Install from Microsoft feed

``` sh
$ declare repo_version=$(if command -v lsb_release &> /dev/null; then lsb_release -r -s; else grep -oP '(?<=^VERSION_ID=).+' /etc/os-release | tr -d '"'; fi)

$ wget https://packages.microsoft.com/config/ubuntu/$repo_version/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
$ sudo dpkg -i packages-microsoft-prod.deb
$ rm packages-microsoft-prod.deb

$ sudo apt update
$ sudo apt install dotnet-sdk-8.0
```

##### Prefer Microsoft feed over Ubuntu

In case you got the SDKs/runtimes messed up with default ones installed from Ubuntu packages and you'd like to wipe all that and install just the Microsoft ones:

``` sh
$ sudo apt remove dotnet* aspnetcore* netstandard*

$ sudo nano /etc/apt/preferences.d/99microsoft-dotnet.pref
```
``` sh
Package: *
Pin: origin "packages.microsoft.com"
Pin-Priority: 1001
```
``` sh
$ sudo apt update
$ sudo apt install dotnet-sdk-8.0
```

#### Install specific version

``` sh
$ apt policy dotnet-sdk-6.0

dotnet-sdk-6.0:
  Installed: 6.0.417-1
  Candidate: 6.0.417-1
  Version table:
 *** 6.0.417-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
        100 /var/lib/dpkg/status
     6.0.416-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.415-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.414-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.413-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.412-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.411-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.410-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.408-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
     6.0.407-1 1001
       1001 https://packages.microsoft.com/ubuntu/22.04/prod jammy/main amd64 Packages
...

$ sudo apt install --allow-downgrades dotnet-sdk-6.0=6.0.415-1
```

#### Delete particular version

Better to use `apt`:

``` sh
$ sudo apt remove dotnet-sdk-6.0 \
    dotnet-apphost-pack-6.0 dotnet-hostfxr-6.0 \
    dotnet-runtime-deps-6.0 dotnet-runtime-6.0 dotnet-targeting-pack-6.0 \
    aspnetcore-runtime-6.0 aspnetcore-targeting-pack-6.0
```

but this might also might work, if needed:

``` sh
$ sudo rm -rf /usr/share/dotnet/sdk/6.0.417/
$ sudo rm -rf /usr/share/dotnet/shared/Microsoft.AspNetCore.App/6.0.25/
$ sudo rm -rf /usr/share/dotnet/shared/Microsoft.NETCore.App/6.0.25/
$ sudo rm -rf /usr/share/dotnet/host/fxr/6.0.25/
```

### Create a new project

Create a folder for a project and go inside:

``` sh
$ mkdir some-project && cd $_
```

Run [dotnet new](https://docs.microsoft.com/en-us/dotnet/core/tools/dotnet-new) with a desired template. It will create a project with a name of the folder you are inside (where you called it from).

#### Specific SDK version

``` sh
$ cd /path/to/somewhere
$ mkdir some-project && cd $_

$ dotnet --list-sdks
2.2.401 [/usr/local/share/dotnet/sdk]
5.0.100 [/usr/local/share/dotnet/sdk]
5.0.402 [/usr/local/share/dotnet/sdk]

$ dotnet new globaljson
$ nano ./global.json
```
``` json
{
  "sdk": {
    "version": "2.2.401"
  }
}
```

Now you can run `donet new`, and it will create a project that targets specified SDK/runtime.

#### MVC project with authentication

``` sh
$ dotnet new mvc --auth Individual
```

### NuGet packages

[NuGet](https://docs.microsoft.com/en-us/nuget/what-is-nuget) is a package manager. To install some package (for example, EntityFrameworkCore MySql from Pomelo) to your project you need to [find its name](https://www.nuget.org) and run:

``` sh
$ dotnet add package Pomelo.EntityFrameworkCore.MySql
```

To remove the package simply run:

``` sh
$ dotnet remove package Pomelo.EntityFrameworkCore.MySql
```

And it never hurts to [restore](https://docs.microsoft.com/en-us/dotnet/core/tools/dotnet-restore) packages now and then, especially if you've just added/removed them to/from the project:

``` sh
$ dotnet restore
```

### Publish the project

``` sh
$ dotnet publish --output "/path/to/deploy/folder/" --configuration release
```
