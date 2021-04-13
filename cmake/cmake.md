## CMake

<!-- MarkdownTOC -->

- [Regular expression](#regular-expression)
- [Read variables from text file](#read-variables-from-text-file)
- [List linked libraries](#list-linked-libraries)

<!-- /MarkdownTOC -->

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

### List linked libraries

``` cmake
get_target_property(linkedLibs ${PROJECT_NAME} LINK_LIBRARIES)
message(STATUS "Linked libraries:")
foreach(l IN LISTS linkedLibs)
    message(STATUS "    * ${l}")
endforeach()
```

Example output:

``` sh
-- Linked libraries:
--     * opengl32
--     * $<LINK_ONLY:OGLCompiler_obj>
--     * $<LINK_ONLY:OSDependent_obj>
--     * $<LINK_ONLY:HLSL_obj>
--     * OGLCompiler_obj
--     * OSDependent_obj
--     * HLSL_obj
--     * $<LINK_ONLY:glslang_obj>
--     * glslang_obj
--     * $<LINK_ONLY:spirv-cross-core_obj>
--     * $<LINK_ONLY:spirv-cross-core_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-hlsl_obj>
--     * $<LINK_ONLY:spirv-cross-msl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-hlsl_obj>
--     * $<LINK_ONLY:spirv-cross-msl_obj>
--     * $<LINK_ONLY:spirv-cross-core_obj>
--     * $<LINK_ONLY:spirv-cross-core_obj>
--     * $<LINK_ONLY:spirv-cross-core_obj>
--     * $<LINK_ONLY:spirv-cross-core_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * $<LINK_ONLY:spirv-cross-glsl_obj>
--     * opengl32
--     * glu32
--     * opengl32
--     * d3d11
--     * d3dcompiler
```
