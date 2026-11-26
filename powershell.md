## PowerShell

<!-- MarkdownTOC -->

- [Exit status code](#exit-status-code)
- [Execution policy](#execution-policy)
    - [Executing another script](#executing-another-script)
- [CLI arguments](#cli-arguments)
    - [Ordinal](#ordinal)
    - [Named](#named)
- [Environment variables](#environment-variables)
    - [Print a variable](#print-a-variable)
    - [Set and unset a variable](#set-and-unset-a-variable)
        - [Unset on Windows](#unset-on-windows)
        - [Unset on Linux / Mac OS](#unset-on-linux--mac-os)
- [Checking if variable is not null or empty](#checking-if-variable-is-not-null-or-empty)
- [String interpolation](#string-interpolation)
- [Path concatenation](#path-concatenation)
- [Replacing](#replacing)
    - [Text inside file](#text-inside-file)
    - [Slashes in Windows path](#slashes-in-windows-path)
- [Logging](#logging)
- [Folder size](#folder-size)
- [Get library architecture](#get-library-architecture)

<!-- /MarkdownTOC -->

### Exit status code

``` powershell
> cmake --build .
> echo $LastExitCode
0
```

### Execution policy

``` powershell
> powershell .\some.ps1
Error message: File .\some.ps1 cannot be loaded because running scripts is disabled on this system. For more information, see about_Execution_Policies at https:/go.microsoft.com/fwlink/?LinkID=135170

> powershell -ExecutionPolicy Bypass -File .\some.ps1
```

#### Executing another script

``` powershell
$VS_DEV_SHELL_2022 = "c:/path/to/visual-studio/Common7/Tools/Launch-VsDevShell.ps1"

& "$VS_DEV_SHELL_2022" | out-null
```

``` powershell
> powershell -ExecutionPolicy Bypass -File .\some.ps1
```

### CLI arguments

#### Ordinal

``` powershell
$something = $args[0]
$another = $args[1]

Write-Output $something
Write-Output $another
```

``` powershell
> .\some.ps1 ololo fuu
ololo
fuu
```

#### Named

The `param()` thing needs to be the absolutely first line in the script, otherwise it fails in various ways.

``` powershell
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

``` powershell
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

#### Print a variable

``` powershell
> echo $env:VCPKG_ROOT
```

``` powershell
Write-Output $env:VCPKG_ROOT
```

or:

``` powershell
$some = $env:VCPKG_ROOT
Write-Output $some
```

#### Set and unset a variable

Set:

``` powershell
> echo $env:some

> $env:some = "ololo"
> echo $env:some
ololo
```

##### Unset on Windows

``` powershell
> rm env:some
> echo $env:some
```

##### Unset on Linux / Mac OS

``` powershell
> ri env:some
> echo $env:some
```

or

``` powershell
> Remove-Item env:some
> echo $env:some
```

### Checking if variable is not null or empty

``` powershell
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

``` powershell
> .\some.ps1
That variable is not set

> $env:VARIABLE_THAT_DOES_NOT_EXIST='ololo'; .\some.ps1; Remove-Item Env:\VARIABLE_THAT_DOES_NOT_EXIST
ololo
```

### String interpolation

Like in Bash, when a string is inside single quotes, it won't expand variables inside it, and so you'll need to use double quotes for that:

``` powershell
$some = 'ololo'

$another1 = 'there is $some sort of things'
Write-Output $another1

$another2 = "there is $some sort of things"
Write-Output $another2
```

``` powershell
> .\some.ps1
there is $some sort of things
there is ololo sort of things
```

### Path concatenation

Note that you might need to escape the backslashes, so for example instead of `c:\some\place` there should be `c:\\some\\place`.

``` powershell
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

``` powershell
> .\some.ps1
c:\some\place\and\another\place
c:\some\place\and\another\place
\and\another\place
```

### Replacing

#### Text inside file

``` powershell
$originalFile = '.\some-original.txt'
$otherFile = '.\some-other.txt'
#(Get-Content $originalFile).replace('ololo', 'fuuuuuu') | Set-Content $originalFile
(Get-Content $originalFile).replace('ololo', 'fuuuuuu') | Set-Content $otherFile
```

``` powershell
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

``` powershell
$windowsPath = 'c:\some\path\to\somewhere.txt'
$properPath = ($windowsPath -replace '\\', '/')

Write-Output $properPath
```

``` powershell
> .\some.ps1
c:/some/path/to/somewhere.txt
```

### Logging

There is a zero-effort out-of-the-box [logging capability](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.host/start-transcript):

``` powershell
# the log file will be place to the location where you called the script from
$LOG_FILE = Join-Path (Get-Item .).FullName "some.log"
# put it somewhere close to the beginning of the script or wherever you want
Start-Transcript -Path "$LOG_FILE"

# optionally stop wherever you want, otherwise it will just stop at the script end
#Stop-Transcript
```

### Folder size

Files count and total size in bytes:

``` powershell
> Get-ChildItem -Recurse 'd:/temp/some' | Measure-Object -Property Length -Sum


Count    : 23771
Average  :
Sum      : 15117560416
Maximum  :
Minimum  :
Property : Length
```

Size value only:

``` powershell
> (Get-ChildItem -Recurse 'd:/temp/some' | Measure-Object -Property Length -Sum).Sum
15117560416
```

### Get library architecture

Using `dumpbin` from [Developer PowerShell](https://learn.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell):

``` powershell
> dumpbin /nologo /headers .\install\windows-x86-msvc145\lib\some.lib | Select-String machine -Raw | Sort-Object -Unique
             14C machine (x86)
```

or with regular PowerShell using this fancy function - since `.lib` is an archive of object files, and each object starts with a 2-byte machine ID, we can read that ID from every member:

``` powershell
> function Get-LibArch {
    param(
        [Parameter(Mandatory, ValueFromPipeline, ValueFromPipelineByPropertyName)]
        [Alias('FullName')][string] $Path
    )
    begin
    {
        $machines = @{ 0x014C = 'x86'; 0x8664 = 'x64'; 0xAA64 = 'ARM64'; 0xA641 = 'ARM64EC'; 0x01C4 = 'ARM' }
        $ascii = [Text.Encoding]::ASCII
    }
    process
    {
        $b = [IO.File]::ReadAllBytes((Convert-Path -LiteralPath $Path))
        if ($b.Length -lt 8 -or $ascii.GetString($b, 0, 8) -ne "!<arch>`n")
        {
            Write-Error "$Path is not a .lib archive"; return
        }
        $archs = [Collections.Generic.SortedSet[string]]::new()
        $count = 0
        $pos = 8
        while ($pos + 60 -le $b.Length) # 60-byte member header
        {
            $name = $ascii.GetString($b, $pos, 16).TrimEnd()
            $size = [int]$ascii.GetString($b, $pos + 48, 10).Trim()
            $data = $pos + 60
            # skip symbol tables ("/"), the long-name table ("//") and "/<...>/" special members
            if ($name -notin '/', '//' -and $name -notlike '/<*' -and $size -ge 8)
            {
                # COFF header: Machine field
                $m = [BitConverter]::ToUInt16($b, $data)
                if ($m -eq 0 -and [BitConverter]::ToUInt16($b, $data + 2) -eq 0xFFFF)
                {
                    # import-library / bigobj members
                    $m = [BitConverter]::ToUInt16($b, $data + 6)
                }
                $label = $machines[[int]$m]
                if (-not $label) { $label = '0x{0:X4}' -f $m }
                [void]$archs.Add($label)
                $count++
            }
            # members are 2-byte aligned
            $pos = $data + $size + ($size % 2)
        }
        [pscustomobject]@{ Arch = $archs -join ', '; Members = $count; Path = $Path }
    }
}
```

And then for a single file:

``` powershell
> Get-LibArch .\install\windows-x86-msvc145\lib\some.lib

Arch Members Path
---- ------- ----
x86        2 C:\code\some\install\windows-x86-msvc145\lib\some.lib
```

or for all the libraries in a given path:

``` powershell
> Get-ChildItem .\install\windows-x86-msvc145 -Recurse -Filter *.lib | Get-LibArch

Arch Members Path
---- ------- ----
x86      146 C:\code\some\install\windows-x86-msvc145\debug\lib\jpegd.lib
x86       17 C:\code\some\install\windows-x86-msvc145\debug\lib\pngd.lib
x86        2 C:\code\some\install\windows-x86-msvc145\debug\lib\somed.lib
...
x86      146 C:\code\some\install\windows-x86-msvc145\lib\jpeg.lib
x86       17 C:\code\some\install\windows-x86-msvc145\lib\png.lib
x86        2 C:\code\some\install\windows-x86-msvc145\lib\some.lib
...
```

No need for `-Recurse` if you point it directly to `\lib`.
