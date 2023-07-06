## CMake

<!-- MarkdownTOC -->

- [CMAKE_SYSTEM_NAME values](#cmake_system_name-values)
- [Regular expression](#regular-expression)
- [Read variables from text file](#read-variables-from-text-file)
- [Build with Clang on Windows](#build-with-clang-on-windows)
    - [With Visual Studio generator](#with-visual-studio-generator)
    - [With Ninja generator](#with-ninja-generator)
- [Download a file](#download-a-file)
- [Viewing evaluated values of generator expressions](#viewing-evaluated-values-of-generator-expressions)
- [Listing include directories](#listing-include-directories)
- [Listing linked libraries](#listing-linked-libraries)

<!-- /MarkdownTOC -->

### CMAKE_SYSTEM_NAME values

<https://gitlab.kitware.com/cmake/cmake/-/issues/21489>

- `AIX`
- `Android`
- `ARTOS`
- `BeOS`
- `BlueGeneP-static`
- `BlueGeneP-dynamic`
- `BlueGeneQ-static`
- `BlueGeneQ-dynamic`
- `BSDOS`
- `Catamount`
- `CrayLinuxEnvironment`
- `CYGWIN`
- `Darwin`
- `DOS`
- `DragonFly`
- `eCos`
- `Euros`
- `FreeBSD`
- `Fuchsia`
- `Generic`
- `Generic-ADSP`
- `Generic-ELF`
- `GHS-MULTI`
- `GNU`
- `Haiku`
- `HP-UX`
- `iOS`
- `kFreeBSD`
- `Linux`
- `Midipix`
- `MirBSD`
- `MP-RAS`
- `MSYS`
- `NetBSD`
- `OpenBSD`
- `OpenVMS`
- `OS2`
- `OSF1`
- `QNX`
- `RISCos`
- `SCO_SV`
- `SINIX`
- `SunOS`
- `syllable`
- `Tru64`
- `tvOS`
- `ULTRIX`
- `UNIX_SV`
- `UnixWare`
- `watchOS`
- `Windows`
- `WindowsCE`
- `WindowsPhone`
- `WindowsStore`
- `Xenix`

That can be used for creating platform-specific conditions, such as:

``` cmake
# to exclude mobile and web platforms
if(
    NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "iOS"
    AND
    NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android"
    AND
    NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore"
    AND
    NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten"
)
    message(STATUS "It's none of the above")
endif()
```

### Regular expression

We have a `git_build_tag` variable which value is `build-2020.03_52025`. We want to extract `52025` part:

```
set(VERSION_BUILD 0)
string(REGEX MATCH "_([0-9]+)$" VERSION_BUILD ${git_build_tag})
message("${CMAKE_MATCH_COUNT}: ${CMAKE_MATCH_1} (${git_build_tag})")
set(VERSION_BUILD ${CMAKE_MATCH_1})
```

### Read variables from text file

A file `version-info.txt`:

```
git_commit_date = 2020-07-22 10:00:43 +0200
git_branch = master
git_build_tag = build-master_52659
```

How to read variables from it:

```
file (STRINGS "version-info.txt" version_info)
foreach(var ${version_info})
  # strip leading spaces
  string(REGEX REPLACE "^[ ]+" "" var ${var})
  # find variable name
  string(REGEX MATCH "^[^ = ]+" varName ${var})
  # find the value
  string(REPLACE "${varName} = " "" varValue ${var})
  # set the variable
  set(${varName} "${varValue}")
  #set_property(GLOBAL PROPERTY ${varName} ${varValue})
endforeach()

message("Build tag: ${git_build_tag}")
```

### Build with Clang on Windows

If you use hardcoded MSVC-specific compilation flags, then building with Clang will fail, but it is possible to use [clang-cl](https://clang.llvm.org/docs/MSVCCompatibility.html), which will try to convert those on the fly. If you don't have hardcoded MSVC-specific flags, then just use `clang.exe`/`clang++.exe`.

#### With Visual Studio generator

From Git BASH:

``` sh
$ mkdir build && cd $_
$ cmake -G "Visual Studio 17 2022" ..
$ cmake --build . --config Release -- -p:CLToolExe=clang-cl.exe -p:CLToolPath="c:/path/to/llvm/bin"
```

#### With Ninja generator

From cmd:

``` cmd
> call c:\path\to\visual-studio\VC\Auxiliary\Build\vcvars64.bat
> mkdir build
> cd build
> cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER:PATH="c:/path/to/llvm/bin/clang-cl.exe" -DCMAKE_CXX_COMPILER:PATH="c:/path/to/llvm/bin/clang-cl.exe" ..
> cmake --build .
```

### Download a file

``` cmake
set(SOME_FILE "some.zip")
message(STATUS "Downloading the file...")
file(DOWNLOAD "https://your.domain/data/${SOME_FILE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/files/${SOME_FILE}"
    #SHOW_PROGRESS
    EXPECTED_HASH SHA1=b28c4cdfd737c41d1252945224aaddf97e45dc27
    STATUS DOWNLOAD_DATASETS_RESULTS
)
list(GET DOWNLOAD_DATASETS_RESULTS 0 DOWNLOAD_STATUS_CODE)
list(GET DOWNLOAD_DATASETS_RESULTS 1 DOWNLOAD_ERROR_MESSAGE)
if(${DOWNLOAD_STATUS_CODE} EQUAL 0)
    message(STATUS "...successfully downloaded the file")
else()
    file(REMOVE "${CMAKE_CURRENT_SOURCE_DIR}/files/${SOME_FILE}")
    message(WARNING "...failed to download the file. ${DOWNLOAD_ERROR_MESSAGE}")
endif()
```

### Viewing evaluated values of generator expressions

Since generator expressions are evaluated only on generation (*so, after configuration*), one cannot print their values during configuration. To view the values you'd need to somehow print them on generation, for example by writing them to a file:

``` cmake
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/dbg/1.txt"
    CONTENT $<TARGET_FILE:${PROJECT_NAME}>
)
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/dbg/2.txt"
    CONTENT $<CONFIG>
)
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/dbg/3.txt"
    CONTENT $<LOWER_CASE:sOMethING-HerE>
)
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/dbg/4.txt"
    CONTENT $<UPPER_CASE:sOMethING-HerE>
)
```

### Listing include directories

With [this project](https://github.com/retifrav/cmake-library-example/blob/b8db42a8d9a6f9610657d90508f66d299fda3d63/internal-project/libraries/SomeLibrary/CMakeLists.txt#L36) as an example:

``` cmake
get_property(inclds
    TARGET ${PROJECT_NAME}
    PROPERTY INCLUDE_DIRECTORIES
)
message(STATUS "Listing include directories for ${PROJECT_NAME}...")
# "dumb" listing with "raw" generator expressions on configuration
foreach(pth ${inclds})
    message(STATUS "  ${pth}")
endforeach()
# actually evaluated generator expressions printed to file on generation
string(JOIN "\n" includeDirectories ${inclds})
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/dbg/include-directories.txt"
    CONTENT ${includeDirectories}
)
```

Dumb listing will output the following:

``` sh
-- Listing include directories for SomeLibrary...
--   /path/to/cmake-library-example/internal-project/libraries/SomeLibrary/src
--   $<BUILD_INTERFACE:/path/to/cmake-library-example/internal-project/libraries/SomeLibrary/include>
--   $<INSTALL_INTERFACE:>
```

And not dumb listing will write the following to `/path/to/cmake-library-example/internal-project/build/dbg/include-directories.txt`:

```
/path/to/cmake-library-example/internal-project/libraries/SomeLibrary/src
/path/to/cmake-library-example/internal-project/libraries/SomeLibrary/include
```

### Listing linked libraries

``` cmake
get_target_property(linkedLibs ${PROJECT_NAME} LINK_LIBRARIES)
message(STATUS "Linked libraries:")
foreach(l IN LISTS linkedLibs)
    message(STATUS "* ${l}")
endforeach()
```

Example output:

``` sh
-- Linked libraries:
-- * opengl32
-- * $<LINK_ONLY:OGLCompiler_obj>
-- * $<LINK_ONLY:OSDependent_obj>
-- * $<LINK_ONLY:HLSL_obj>
-- * OGLCompiler_obj
-- * OSDependent_obj
-- * HLSL_obj
-- * $<LINK_ONLY:glslang_obj>
-- * glslang_obj
-- * $<LINK_ONLY:spirv-cross-core_obj>
-- * $<LINK_ONLY:spirv-cross-core_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-hlsl_obj>
-- * $<LINK_ONLY:spirv-cross-msl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-hlsl_obj>
-- * $<LINK_ONLY:spirv-cross-msl_obj>
-- * $<LINK_ONLY:spirv-cross-core_obj>
-- * $<LINK_ONLY:spirv-cross-core_obj>
-- * $<LINK_ONLY:spirv-cross-core_obj>
-- * $<LINK_ONLY:spirv-cross-core_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * $<LINK_ONLY:spirv-cross-glsl_obj>
-- * opengl32
-- * glu32
-- * opengl32
-- * d3d11
-- * d3dcompiler
```
