## sh

- [Several arguments of the same option](#several-arguments-of-the-same-option)

### Several arguments of the same option

Pay attention to brackets - that is how you set an array of values.

```
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

```
$ ./some.sh -g Xcode -d "ON_BUILD_BOT=ON" -d "ENABLE_IOS=ON"
```
