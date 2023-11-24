cmake_minimum_required(VERSION 3.22)

project("some")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Filename postfix for libraries under DEBUG configuration")

#set(CMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL Release)
#set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

add_executable(${CMAKE_PROJECT_NAME})

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        main.cpp
)

find_package(GDAL REQUIRED)

set(GDAL_VARIABLES
    GDAL_FOUND
    GDAL_VERSION
    GDAL_INCLUDE_DIR
    GDAL_INCLUDE_DIRS
    GDAL_LIBRARY
    GDAL_LIBRARIES
    GDAL_DLLS
    GDAL_DATA_DIR
)

foreach(GDAL_VARIABLE ${GDAL_VARIABLES})
    message("---")
    message(STATUS "${GDAL_VARIABLE}: ${${GDAL_VARIABLE}}")
    if(DEFINED CACHE{${GDAL_VARIABLE}})
        message(STATUS "${GDAL_VARIABLE} is a cache variable")
    else()
        message(STATUS "${GDAL_VARIABLE} is not a cache variable")
    endif()
endforeach()
message("---")

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        GDAL::GDAL
)
