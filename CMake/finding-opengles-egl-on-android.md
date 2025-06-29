## Finding OpenGL ES and EGL on Android

Haven't actually tried this, but:

``` cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    # GLESv2
    find_path(GLES2_INCLUDE_DIR
        GLES2/gl2.h
        HINTS ${ANDROID_NDK}
    )
    find_library(GLES2_LIBRARY
        libGLESv2.so
        HINTS ${GLES2_INCLUDE_DIR}/../lib
    )
    #target_include_directories(${PROJECT_NAME} PUBLIC ${GLES2_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GLES2_LIBRARY})

    # EGL
    find_path(EGL_INCLUDE_DIR
        EGL/egl.h
        HINTS ${ANDROID_NDK}
    )
    find_library(EGL_LIBRARY
        libEGL.so
        HINTS ${EGL_INCLUDE_DIR}/../lib
    )
    #target_include_directories(${PROJECT_NAME} PUBLIC ${EGL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${EGL_LIBRARY})
endif()
```
