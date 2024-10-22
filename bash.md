## Bash

Here's also a [small collection](https://github.com/retifrav/bash-scripts) of performing various tasks.

<!-- MarkdownTOC -->

- [Append to array and iterate](#append-to-array-and-iterate)
- [Several arguments of the same option](#several-arguments-of-the-same-option)
- [Print a symbol 5 times](#print-a-symbol-5-times)
- [Increment a variable](#increment-a-variable)
- [Files names lengths](#files-names-lengths)
- [Regular expressions](#regular-expressions)
- [Current script path](#current-script-path)
- [Read from file to variable](#read-from-file-to-variable)
    - [Via source](#via-source)
    - [Via reading](#via-reading)
- [Exit from subshells and preserve exit code](#exit-from-subshells-and-preserve-exit-code)

<!-- /MarkdownTOC -->

### Append to array and iterate

``` sh
foo=(a b c)
foo=("${foo[@]}" d)
for i in "${foo[@]}"; do echo "$i" ; done
```

### Several arguments of the same option

Pay attention to brackets - that is how you set an array of values.

``` sh
#!/bin/bash

# just to show the difference between an option with single argument
cmakeGenerator="[unknown]"
# and an option with several arguments
cmakeFlags=()

while getopts ":g:d:" opt; do
  case $opt in
    g) cmakeGenerator="$OPTARG"
    ;;
    d) cmakeFlags+=("$OPTARG")
    ;;
    \?)
        echo "Unknown option -$OPTARG" >&2
        exit 1
    ;;
  esac
done

# unwrap the flags to string
flags=""
if [ -z "$cmakeFlags" ]; then
    echo "No CMake flags provided"
else
    for f in "${cmakeFlags[@]}"; do
        flags+="-D$f "
    done
    echo "CMake flags: $flags"
fi

cmake -G "$cmakeGenerator" $flags .. && cmake --build . --config Release
```

Now you can call this script like this:

``` sh
$ ./some.sh -g Xcode -d "ON_BUILD_BOT=ON" -d "ENABLE_IOS=ON"
```

### Print a symbol 5 times

``` sh
printf %5s | tr " " "="
```

### Increment a variable

To perform arithmetic operations in bash you need to use `$((...))``:

``` sh
x=2
x=$((x + 1))
echo $x
```

or:

``` sh
y=2
((y++))
echo $y
```

### Files names lengths

Top 5 longest file names in the current folder recursively:

``` sh
find . | awk 'function base(f){sub(".*/", "", f); return f;} {print length(base($0)), $0}'| sort -nr | head -5
```

Or just the length of the longest one:

``` sh
find . | awk 'function base(f){sub(".*/", "", f); return f;} {print length(base($0))}'| sort -nr | head -1
```

### Regular expressions

``` sh
tempFolderRegEx='.*\/TemporaryDirectory\.[a-zA-Z0-9]+$'
currentPath=$(pwd)
echo $currentPath
if [[ "$currentPath" =~ $tempFolderRegEx ]]; then
    echo 'This is a TemporaryDirectory'
else
    echo 'This is something else'
fi
```

### Current script path

Doesn't matter where you called the script from:

``` sh
# path to the script's folder
scriptDir=$(dirname -- "$BASH_SOURCE")

# script file name
scriptFile=$(basename "$0")
```

### Read from file to variable

#### Via source

File `some-vars.sh`:

```
x="1.2.3"
```

The script:

``` sh
#!/bin/bash

source ./some-vars.sh
echo $x
```

#### Via reading

File `some-var.txt`:

```
1.2.3
```

The script:

``` sh
#!/bin/bash

# reading
#x=$(cat ./some-var.txt)

# or redirecting
x=$(<./some-var.txt)

if [ -z "$x" ]; then
    echo "[ERROR] Could not read the variable from file"
    exit 1
fi

echo $x
```

### Exit from subshells and preserve exit code

``` sh
#!/bin/bash

set -E
trap 'exit_code=$? && [[ "$exit_code" -ne 11 && "$exit_code" -ne 12 ]] || exit $exit_code' ERR

if [[ "$1" == 'thing' ]]; then
    printf "[$1] equals [thing]\n" >&2
    exit 1
fi

curl -s -w "\n%{http_code}" 'https://swapi.dev/api/people/1?format=json' | {
    read httpResponseBody # must be read first
    read httpStatusCode # must be read second

    if [[ -z "$httpStatusCode" || "$httpStatusCode" != '200' ]]; then
        printf "HTTP status code: $httpStatusCode\n" >&2
        exit 11
    fi

    someName=$(jq -r .name <<< "$httpResponseBody")
    otherName="$1"
    if [[ "$someName" != "$otherName" ]]; then
        printf "[$someName] is not [$otherName]\n" >&2
        exit 12
    fi
}

exit 0
```
``` sh
$ ./some.sh thing
[thing] equals [thing]
$ echo $?
1

$ ./some.sh ololo
[Luke Skywalker] is not [ololo]
$ echo $?
12

$ ./some.sh "Luke Skywalker"
$ echo $?
0
```
