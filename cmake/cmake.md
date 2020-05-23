## CMake

### Regular expression

We have a `git_build_tag` variable which value is `build-2020.03_52025`. We want to extract `52025` part:

```
set(VERSION_BUILD 0)
string(REGEX MATCH "_([0-9]+)$" VERSION_BUILD ${git_build_tag})
message("${CMAKE_MATCH_COUNT}: ${CMAKE_MATCH_1} (${git_build_tag})")
set(VERSION_BUILD ${CMAKE_MATCH_1})
```
