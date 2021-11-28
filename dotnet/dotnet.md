## dotnet

Set of commands for [dotnet](https://docs.microsoft.com/en-us/dotnet/core/tools/dotnet) (.NET Core CLI utility). All the commands are applied to the current directory, so you should run those from where you `.csproj` is (or will be, in case of `dotnet new`).

<!-- MarkdownTOC -->

- [Maintenance](#maintenance)
    - [List installed SDKs](#list-installed-sdks)
    - [List installed runtimes](#list-installed-runtimes)
    - [Delete SDK and runtime](#delete-sdk-and-runtime)
- [Create new project](#create-new-project)
    - [Specific SDK version](#specific-sdk-version)
    - [MVC project with authentication](#mvc-project-with-authentication)
- [NuGet packages](#nuget-packages)
- [Publish the project](#publish-the-project)

<!-- /MarkdownTOC -->

### Maintenance

#### List installed SDKs

``` bash
dotnet --list-sdks
```

#### List installed runtimes

``` bash
dotnet --list-runtimes
```

#### Delete SDK and runtime

``` bash
sudo rm -rf /usr/local/share/dotnet/sdk/2.1.403/
sudo rm -rf /usr/local/share/dotnet/shared/Microsoft.AspNetCore.All/2.1.5/
sudo rm -rf /usr/local/share/dotnet/shared/Microsoft.AspNetCore.App/2.1.5/
sudo rm -rf /usr/local/share/dotnet/host/fxr/2.1.5/
```

### Create new project

Create a folder for a project and go inside:

``` sh
$ mkdir some-project && cd "$_"
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

``` bash
dotnet add package Pomelo.EntityFrameworkCore.MySql
```

To remove the package simply run:

``` bash
dotnet remove package Pomelo.EntityFrameworkCore.MySql
```

And it never hurts to [restore](https://docs.microsoft.com/en-us/dotnet/core/tools/dotnet-restore) packages now and then, especially if you've just added/removed them to/from the project:

``` bash
dotnet restore
```

### Publish the project

``` bash
dotnet publish --output "/path/to/deploy/folder/" --configuration release
```
