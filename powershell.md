## PowerShell

<!-- MarkdownTOC -->

- [Execution policy](#execution-policy)
    - [Executing another script](#executing-another-script)
- [CLI arguments](#cli-arguments)
    - [Ordinal](#ordinal)
    - [Named](#named)
- [Environment variables](#environment-variables)
- [Checking if variable is not null or empty](#checking-if-variable-is-not-null-or-empty)
- [String interpolation](#string-interpolation)
- [Path concatenation](#path-concatenation)
- [Replacing](#replacing)
    - [Text inside file](#text-inside-file)
    - [Slashes in Windows path](#slashes-in-windows-path)
- [Logging](#logging)

<!-- /MarkdownTOC -->

### Execution policy

``` ps
> powershell .\some.ps1
Error message: File .\some.ps1 cannot be loaded because running scripts is disabled on this system. For more information, see about_Execution_Policies at https:/go.microsoft.com/fwlink/?LinkID=135170

> powershell -ExecutionPolicy Bypass -File .\some.ps1
```

#### Executing another script

``` ps
$VS_DEV_SHELL_2022 = "c:/path/to/visual-studio/Common7/Tools/Launch-VsDevShell.ps1"

& "$VS_DEV_SHELL_2022" | out-null
```

``` cmd
> powershell -ExecutionPolicy Bypass -File .\some.ps1
```

### CLI arguments

#### Ordinal

``` ps
$something = $args[0]
$another = $args[1]

Write-Output $something
Write-Output $another
```

``` cmd
> .\some.ps1 ololo fuu
ololo
fuu
```

#### Named

The `param()` thing needs to be the absolutely first line in the script, otherwise it fails in various ways.

``` ps
param(
    [string]
    $SOME_SOURCE_PATH = (Get-Item .).FullName,

    [string]
    $BUILD_TYPE = "Release",

    [string]
    $PLATFORM_NAME = "MAUI",

    [string]
    $DOTNET_FRAMEWORK = "net8.0-windows10.0.22621.0",

    [string]
    $NUGET_SOURCE = "https://api.nuget.org/v3/index.json",

    [string]
    $VS_DEV_SHELL_2022 = $env:VS_DEV_SHELL_2022, # c:/path/to/visual-studio/Common7/Tools/Launch-VsDevShell.ps1

    [string]
    $ANDROID_SDK_PATH = "c:/path/to/android-sdk"
)

if ($MyInvocation.BoundParameters.Keys.Count -gt 0)
{
    echo "Overridden parameters:"
    foreach ($k in $MyInvocation.BoundParameters.Keys)
    {
        $v = (Get-Variable -Name $k).Value; # -ErrorAction SilentlyContinue;
        echo "- ${k}: $v"
    }
    echo ""
}

echo "Parameters summary:"
foreach ($k in $MyInvocation.MyCommand.Parameters.Keys)
{
    $v = (Get-Variable -Name $k).Value; # -ErrorAction SilentlyContinue;
    echo "- ${k}: $v"
}
echo ""

if ([string]::IsNullOrEmpty($VS_DEV_SHELL_2022))
{
    echo "[ERROR] You need to provide the path to Visual Studio Developer PowerShell script (Launch-VsDevShell.ps1)"
    exit 1
}
elseif (-Not (Test-Path "$VS_DEV_SHELL_2022"))
{
    echo "[ERROR] Provided path to Visual Studio Developer PowerShell script ($VS_DEV_SHELL_2022) does not exist"
    exit 2
}

$SOME_INSTALL_PATH = Join-Path "$SOME_SOURCE_PATH" install

# stop execution on the first error. Doesn't help with executables, unfortunately,
# as it only applies to PowerShell's own commands/functions
# https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_preference_variables#erroractionpreference
$ErrorActionPreference = "Stop"

# ---

#ls $SOME_INSTALL_PATH
```

``` cmd
> .\some.ps1 -PLATFORM_NAME "Xamarin" -BUILD_TYPE Debug
Overridden parameters:
- PLATFORM_NAME: Xamarin
- BUILD_TYPE: Debug

Parameters summary:
- SOME_SOURCE_PATH: c:\some\place
- BUILD_TYPE: Debug
- PLATFORM_NAME: Xamarin
- DOTNET_FRAMEWORK: net8.0-windows10.0.22621.0
- NUGET_SOURCE: https://api.nuget.org/v3/index.json
- VS_DEV_SHELL_2022: c:/path/to/visual-studio\Common7\Tools\Launch-VsDevShell.ps1
- ANDROID_SDK_PATH: c:/path/to/android-sdk
```

### Environment variables

``` ps
Write-Output $env:VCPKG_ROOT

# or

$some = $env:VCPKG_ROOT
Write-Output $some
```

``` cmd
> .\some.ps1
d:\programs\vcpkg
d:\programs\vcpkg
```

### Checking if variable is not null or empty

``` ps
$some = $env:VARIABLE_THAT_DOES_NOT_EXIST
if (-not ([string]::IsNullOrEmpty($some)))
{
    Write-Output $some
}
else
{
    Write-Output 'That variable is not set'
}
```

``` cmd
> .\some.ps1
That variable is not set

> $env:VARIABLE_THAT_DOES_NOT_EXIST='ololo'; .\some.ps1; Remove-Item Env:\VARIABLE_THAT_DOES_NOT_EXIST
ololo
```

### String interpolation

Like in Bash, when a string is inside single quotes, it won't expand variables inside it, and so you'll need to use double quotes for that:

``` ps
$some = 'ololo'

$another1 = 'there is $some sort of things'
Write-Output $another1

$another2 = "there is $some sort of things"
Write-Output $another2
```

``` cmd
> .\some.ps1
there is $some sort of things
there is ololo sort of things
```

### Path concatenation

Note that you might need to escape the backslashes, so for example instead of `c:\some\place` there should be `c:\\some\\place`.

``` ps
# without trailing/leading slashes
$somePath1 = "c:\some\place"
$anotherPath1 = [IO.Path]::Combine($somePath1, 'and\another\place')
Write-Output $anotherPath1

# with trailing slash
$somePath2 = "c:\some\place\"
$anotherPath2 = [IO.Path]::Combine($somePath2, 'and\another\place')
Write-Output $anotherPath2

# with leading slash in the second component
$somePath3 = "c:\some\place"
$anotherPath3 = [IO.Path]::Combine($somePath3, '\and\another\place')
# note how resulting path doesn't have the first component anymore
Write-Output $anotherPath3
```

``` cmd
> .\some.ps1
c:\some\place\and\another\place
c:\some\place\and\another\place
\and\another\place
```

### Replacing

#### Text inside file

``` ps
$originalFile = '.\some-original.txt'
$otherFile = '.\some-other.txt'
#(Get-Content $originalFile).replace('ololo', 'fuuuuuu') | Set-Content $originalFile
(Get-Content $originalFile).replace('ololo', 'fuuuuuu') | Set-Content $otherFile
```

``` cmd
> cat .\some-original.txt
there is some ololo
in ololo sort of things
and with ololo on top of it

> .\some.ps1

> cat .\some-other.txt
there is some fuuuuuu
in fuuuuuu sort of things
and with fuuuuuu on top of it
```

#### Slashes in Windows path

``` ps
$windowsPath = 'c:\some\path\to\somewhere.txt'
$properPath = ($windowsPath -replace '\\', '/')

Write-Output $properPath
```

``` cmd
> .\some.ps1
c:/some/path/to/somewhere.txt
```

### Logging

There is a zero-effort out-of-the-box [logging capability](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.host/start-transcript):

``` ps
# the log file will be place to the location where you called the script from
$LOG_FILE = Join-Path (Get-Item .).FullName "some.log"
# put it somewhere close to the beginning of the script or wherever you want
Start-Transcript -Path "$LOG_FILE"

# optionally stop wherever you want, otherwise it will just stop at the script end
#Stop-Transcript
```

