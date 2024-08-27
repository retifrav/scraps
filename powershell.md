## PowerShell

<!-- MarkdownTOC -->

- [Execution policy](#execution-policy)
- [CLI arguments](#cli-arguments)
- [Environment variables](#environment-variables)
- [Checking if variable is not null or empty](#checking-if-variable-is-not-null-or-empty)
- [String interpolation](#string-interpolation)
- [Path concatenation](#path-concatenation)
- [Replacing](#replacing)
    - [Text inside file](#text-inside-file)
    - [Slashes in Windows path](#slashes-in-windows-path)

<!-- /MarkdownTOC -->

### Execution policy

``` ps
> powershell .\some.ps1
Error message: File .\some.ps1 cannot be loaded because running scripts is disabled on this system. For more information, see about_Execution_Policies at https:/go.microsoft.com/fwlink/?LinkID=135170

> powershell -ExecutionPolicy Bypass -File .\some.ps1
```

### CLI arguments

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
