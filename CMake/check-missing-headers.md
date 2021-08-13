## Check for missing headers

Based on <https://stackoverflow.com/a/57900525/1688203>.

In a project like this:

``` sh
|-CMakeLists.txt
|-include
|  |-hdr.hpp    # includes a non-existent header
|  |-headr.h    # includes a non-existent header
|  |-heared.h   # doesn't include any header
|  |-nstd
|  |  |-nshdr.h # includes a non-existent header
```

`CMakeLists.txt`:

``` cmake
cmake_minimum_required(VERSION 3.15)
project(headers_checker)

include_directories(
    "${CMAKE_SOURCE_DIR}/include"
)
#message(STATUS "Include path: ${CMAKE_SOURCE_DIR}/include")

add_custom_target(chkhdrs)

file(GLOB_RECURSE headers_root "include/*.h" "include/*.hpp")
set(headers_counter 0)
FOREACH(hdr ${headers_root})
    MATH(EXPR headers_counter "${headers_counter}+1")
    #message(STATUS "Header #${headers_counter}: ${hdr}")
    get_filename_component(hdr_we ${hdr} NAME_WE)
    SET(CHK_TARGET "${hdr_we}-${headers_counter}.chk")
    #message(STATUS "Target: ${CHK_TARGET}")
    add_custom_target(
        ${CHK_TARGET}
        COMMAND ${CMAKE_CXX_COMPILER} -c ${hdr} -Wno-deprecated
        VERBATIM
    )
    add_dependencies(chkhdrs ${CHK_TARGET})
ENDFOREACH()
```

Trying to build the project results in errors:

``` sh
$ mkdir build && $_
$ cmake -G Ninja ..

$ cmake --build . --target chkhdrs
[1/4] cmd.exe /C "cd /D D:\temp\hdrs\build && D:\programs\llvm\bin\clang++.exe -c D:/temp/hdrs/include/headr.h -Wno-deprecated"
FAILED: CMakeFiles/headr-2.chk
cmd.exe /C "cd /D D:\temp\hdrs\build && D:\programs\llvm\bin\clang++.exe -c D:/temp/hdrs/include/headr.h -Wno-deprecated"
D:/temp/hdrs/include/headr.h:1:10: fatal error: 's.h' file not found
#include "s.h"
         ^~~~~
1 error generated.
[2/4] cmd.exe /C "cd /D D:\temp\hdrs\build && D:\programs\llvm\bin\clang++.exe -c D:/temp/hdrs/include/nstd/nshdr.h -Wno-deprecated"
FAILED: CMakeFiles/nshdr-4.chk
cmd.exe /C "cd /D D:\temp\hdrs\build && D:\programs\llvm\bin\clang++.exe -c D:/temp/hdrs/include/nstd/nshdr.h -Wno-deprecated"
D:/temp/hdrs/include/nstd/nshdr.h:1:10: fatal error: 'gfdfg.h' file not found
#include "gfdfg.h"
         ^~~~~~~~~
1 error generated.
[3/4] cmd.exe /C "cd /D D:\temp\hdrs\build && D:\programs\llvm\bin\clang++.exe -c D:/temp/hdrs/include/hdr.hpp -Wno-deprecated"
FAILED: CMakeFiles/hdr-1.chk
cmd.exe /C "cd /D D:\temp\hdrs\build && D:\programs\llvm\bin\clang++.exe -c D:/temp/hdrs/include/hdr.hpp -Wno-deprecated"
D:/temp/hdrs/include/hdr.hpp:1:10: fatal error: 'some.h' file not found
#include "some.h"
         ^~~~~~~~
1 error generated.
[4/4] cmd.exe /C "cd /D D:\temp\hdrs\build && D:\programs\llvm\bin\clang++.exe -c D:/temp/hdrs/include/heared.h -Wno-deprecated"
ninja: build stopped: subcommand failed.

$ echo $?
1
```
