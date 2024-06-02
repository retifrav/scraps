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
- [Passing CLI arguments with Windows paths](#passing-cli-arguments-with-windows-paths)
- [Reuse linked libraries of a target](#reuse-linked-libraries-of-a-target)
- [Threads discovery on Mac OS](#threads-discovery-on-mac-os)
- [Copying DLLs on installation](#copying-dlls-on-installation)
- [Removing a redundant interface link library](#removing-a-redundant-interface-link-library)
- [Detect Clang behind MSVC](#detect-clang-behind-msvc)

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

### Passing CLI arguments with Windows paths

Say you have a TeamCity build in which you'd like to set `CMAKE_PREFIX_PATH` or some other path using `%teamcity.build.checkoutDir%` TeamCity variable, for example something like `%teamcity.build.checkoutDir%/dependencies/something`. On Windows agents this will evaluate to a monster value like this one: `d:\buildagent\work\some-build/dependencies/something`. But even if slashes were consistently all backslashes, that would cause troubles anyway.

So one thing you can do is replace backslashes with normal slashes, for example [like this](https://github.com/retifrav/scraps/blob/master/_windows/cmd.md#normalize-windows-path). Another thing, which is what you should probably do instead, is to provide those paths as [`-D <var>:<type>=<value>`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-D), so:

``` cmake
-DCMAKE_PREFIX_PATH:PATH="%teamcity.build.checkoutDir%/dependencies/something"
```

And then CMake will take care of forming a correct path from this.

Don't forget that TeamCity uses single `%` for its own variables, which is the same what Windows uses for environment variables, so if you'll need to use a Windows variable in TeamCity, then it needs to be `%%` instead of a single `%`, so for example `%%PATH%%` or `%%VCPKG_ROOT%%`.

### Reuse linked libraries of a target

If you'd like to get a list of libraries used in one target (*the main project target, for example*) and link to them in another target:

``` cmake
# if you'd like to take a look at the list
#file(GENERATE
#    OUTPUT "${CMAKE_BINARY_DIR}/dbg/linked-libraries.txt"
#    CONTENT $<TARGET_PROPERTY:${CMAKE_PROJECT_NAME},LINK_LIBRARIES>
#)

target_link_libraries("AnotherTargetName"
    PRIVATE
        $<TARGET_PROPERTY:${CMAKE_PROJECT_NAME},LINK_LIBRARIES>
)
```

### Threads discovery on Mac OS

Trying to cross-compile for Emscripten on Mac OS on ARM (Apple silicon) you might get an error about Threads discovery:

```
-- ZSTD_MULTITHREAD_SUPPORT is enabled
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Check if compiler accepts -pthread
-- Check if compiler accepts -pthread - no
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
CMake Error at /Users/vasya/programs/vcpkg/downloads/tools/cmake-3.27.1-osx/cmake-3.27.1-macos-universal/CMake.app/Contents/share/cmake-3.27/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find Threads (missing: Threads_FOUND)
Call Stack (most recent call first):
  /Users/vasya/programs/vcpkg/downloads/tools/cmake-3.27.1-osx/cmake-3.27.1-macos-universal/CMake.app/Contents/share/cmake-3.27/Modules/FindPackageHandleStandardArgs.cmake:600 (_FPHSA_FAILURE_MESSAGE)
  /Users/vasya/programs/vcpkg/downloads/tools/cmake-3.27.1-osx/cmake-3.27.1-macos-universal/CMake.app/Contents/share/cmake-3.27/Modules/FindThreads.cmake:226 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
  /Users/vasya/programs/vcpkg/scripts/buildsystems/vcpkg.cmake:859 (_find_package)
  CMakeLists.txt:139 (find_package)
```

The workaround might be this:

``` cmake
set(THREADS_PREFER_PTHREAD_FLAG 1)
set(THREADS_LIBRARIES_NAME "Threads::Threads")
if(CMAKE_CROSSCOMPILING AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND EMSCRIPTEN)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_LIBRARIES_NAME "-lpthread")
    set(CMAKE_THREAD_LIBS_INIT ${THREADS_LIBRARIES_NAME})
else()
    find_package(Threads REQUIRED)
endif()

# ...

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${THREADS_LIBRARIES_NAME}
)
```

But I don't know if the resulting build will work fine in consuming projects.

### Copying DLLs on installation

A poor man's DLLs deployment crutch:

``` cmake
include(GNUInstallDirs)

set(EXECUTABLE_INSTALLATION_PATH ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME})

if(APPLE)
    install(TARGETS ${CMAKE_PROJECT_NAME}
        BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
else()
    install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION ${EXECUTABLE_INSTALLATION_PATH}
    )
endif()

# if some dependencies are built as SHARED
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    #file(GENERATE OUTPUT debug/dlls.txt CONTENT $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}>)
    
    if(NOT EXISTS ${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_INSTALLATION_PATH}) # it doesn't exist before the very first installation yet
        #install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_INSTALLATION_PATH}) # still too late, because add_custom_command() will execute before that
        file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_INSTALLATION_PATH}) # but at least this one will execute before add_custom_command()
    endif()
    
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}> # locate DLLs (might discover some redundant ones too)
            ${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_INSTALLATION_PATH} # where to copy them
        COMMAND_EXPAND_LISTS
        COMMENT "Copying runtime dependencies (DLLs)"
    )
endif()
```

### Removing a redundant interface link library

Sometimes you have a library, which your library links to, but you don't want this linking to propagate to consuming projects:

``` cmake
# a dirty(?) hack to remove SOME_LIBRARY_WHICH_ISNT_NEEDED_FOR_CONSUMING_PROJECT from INTERFACE_LINK_LIBRARIES,
# as it isn't actually needed, and there isn't(?) "clean" mechanism for doing that
#
# from CMake 3.27 we will be able to use $<COMPILE_ONLY:...>, which should be better way
#
get_target_property(YOURLIBRARYTARGETHERE_INTERFACE_LINK_LIBRARIES ${PROJECT_NAME} INTERFACE_LINK_LIBRARIES)
if(NOT "${YOURLIBRARYTARGETHERE_INTERFACE_LINK_LIBRARIES}" STREQUAL "YOURLIBRARYTARGETHERE_INTERFACE_LINK_LIBRARIES-NOTFOUND")
    # foreach(ill IN ITEMS ${YOURLIBRARYTARGETHERE_INTERFACE_LINK_LIBRARIES})
    #     message(STATUS "ill: ${ill}")
    # endforeach()
    list(REMOVE_ITEM YOURLIBRARYTARGETHERE_INTERFACE_LINK_LIBRARIES [=[$<LINK_ONLY:SOME_LIBRARY_WHICH_ISNT_NEEDED_FOR_CONSUMING_PROJECT>]=])
    # foreach(ill IN ITEMS ${YOURLIBRARYTARGETHERE_INTERFACE_LINK_LIBRARIES})
    #     message(STATUS "ill: ${ill}")
    # endforeach()
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
            INTERFACE_LINK_LIBRARIES "${YOURLIBRARYTARGETHERE_INTERFACE_LINK_LIBRARIES}"
    )
endif()
```

### Detect Clang behind MSVC

- <https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html>
- <https://stackoverflow.com/a/10055571/1688203>
- <https://discourse.cmake.org/t/clang-on-windows-detected-with-msvc-interface/3470/3>

``` cmake
if(
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
    OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
    OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
)
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /W1
    )
else()
    message(WARNING "Got an unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") # for MSVC and clang-cl
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            _USE_MATH_DEFINES
            _CRT_SECURE_NO_WARNINGS
    )
endif()
```
