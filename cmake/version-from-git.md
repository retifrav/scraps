## Getting build number from Git tag

Suppose you tag builds in your CI/CD. Perhaps you have several platforms and every build should share the same common build number. So you can tag the latest commit on the first step (with something like `build-12345`) of the build-chain and all the consequent steps then will be able to extract it using Git.

```
set(APP_BUILD_TAG "00000")
# one more way to get the absolute last tag: git describe --tags $(git rev-list --tags --max-count=1)
execute_process(
    COMMAND git describe --tags --abbrev=0 --match "build-*"
    WORKING_DIRECTORY ${ROOT_DIR}
    RESULT_VARIABLE gitResult
    ERROR_VARIABLE gitError
    OUTPUT_VARIABLE gitOut
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT gitResult EQUAL 0)
    message("-! Git command status: ${gitResult}")
    if(NOT gitError STREQUAL "")
        message("-! Error: ${gitError}")
    endif()
    message("- Couldn't extract build number from Git tag. Will fallback to default value")
else()
    #message(${gitOut})
    string(REPLACE "build-" "" gitBuildNumber ${gitOut})
    set(APP_BUILD_TAG ${gitBuildNumber})
    file(WRITE "${PROJECT_BINARY_DIR}/include/build_info.h" "int buildTag = ${APP_BUILD_TAG};")
endif()
message("- Build number (from Git tag): ${APP_BUILD_TAG}")

# so this header is visible everywhere in the project sources
include_directories(
    "${PROJECT_BINARY_DIR}/include"
)

# you can use right away to set the project version
set(target SomeApp)
project(${target} VERSION 1.0.${APP_BUILD_TAG})
```

And then in one of the sources:

```
#include <build_info.h>

int buildNumber = buildTag;
```
