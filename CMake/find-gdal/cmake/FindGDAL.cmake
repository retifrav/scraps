# Distributed under the OSI-approved GNU General Public License v3.0. See
# https://www.gnu.org/licenses/gpl-3.0.en.html for details.

#[=======================================================================[.rst:
FindGDAL
--------

Finds Geospatial Data Abstraction Library (GDAL).

Imported Targets
^^^^^^^^^^^^^^^^

If GDAL is found, the following imported target will be provided:

``GDAL::GDAL``
  The GDAL library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``GDAL_FOUND``
  True if the system has the GDAL library.
``GDAL_VERSION``
  The version of GDAL found.
``GDAL_INCLUDE_DIRS``
  Include directories needed to use GDAL.
``GDAL_LIBRARIES``
  Libraries needed to link to GDAL.
``GDAL_DLLS``
  DLL files for GDAL and its dependencies.
``GDAL_DATA_DIR``
  GDAL data files.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables will also be set:

``GDAL_INCLUDE_DIR``
  The directory containing GDAL public headers.
``GDAL_LIBRARY``
  The path to the GDAL library.

#]=======================================================================]

set(PATH_TO_GDAL "${CMAKE_PREFIX_PATH}")
if(DEFINED GDAL_DIR)
    set(PATH_TO_GDAL "${GDAL_DIR}")
elseif(DEFINED GDAL_ROOT)
    set(PATH_TO_GDAL "${GDAL_ROOT}")
endif()

find_path(GDAL_INCLUDE_DIR
    gdal.h
    "${PATH_TO_GDAL}/include/gdal"
    NO_CMAKE_FIND_ROOT_PATH
)

if(EXISTS "${GDAL_INCLUDE_DIR}/gdal_version.h")
    file(STRINGS "${GDAL_INCLUDE_DIR}/gdal_version.h" _gdal_version
        REGEX "GDAL_RELEASE_NAME")
    string(REGEX REPLACE ".*\"\(.*\)\"" "\\1" GDAL_VERSION "${_gdal_version}")
    unset(_gdal_version)
else()
    set(GDAL_VERSION GDAL_VERSION-NOTFOUND)
endif()

find_library(GDAL_LIBRARY
    NAMES gdal_i
    PATHS "${PATH_TO_GDAL}/lib"
    NO_CMAKE_FIND_ROOT_PATH
)

find_file(GDAL_LIBRARY_DLL
    # there is no wildcards for names, and there is seemingly no logic behind this versioning,
    # so hardcoding it is
    NAMES gdal303.dll gdal304.dll gdal305.dll # ...
    PATHS "${PATH_TO_GDAL}/bin"
    NO_CMAKE_FIND_ROOT_PATH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GDAL
    FOUND_VAR GDAL_FOUND
    REQUIRED_VARS
        GDAL_INCLUDE_DIR
        GDAL_LIBRARY
        GDAL_LIBRARY_DLL
    VERSION_VAR GDAL_VERSION
)

if(GDAL_FOUND)
    set(GDAL_LIBRARIES ${GDAL_LIBRARY})
    cmake_path(GET GDAL_INCLUDE_DIR PARENT_PATH GDAL_INCLUDE_DIRS) # set(GDAL_INCLUDE_DIRS ${GDAL_INCLUDE_DIR}/..)

    file(GLOB GDAL_DLLS
        "${PATH_TO_GDAL}/bin/gdal*.dll"
        "${PATH_TO_GDAL}/share/gdal/dlls/*.dll"
    )
    set(GDAL_DATA_DIR "${PATH_TO_GDAL}/share/gdal/data")

    if(NOT TARGET GDAL::GDAL)
        add_library(GDAL::GDAL SHARED IMPORTED)
        set_target_properties(GDAL::GDAL PROPERTIES
            IMPORTED_LOCATION "${GDAL_LIBRARY_DLL}"
            IMPORTED_IMPLIB "${GDAL_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GDAL_INCLUDE_DIRS}"
        )
        # or
        # add_library(GDAL::GDAL SHARED IMPORTED)
        # # Release configuration properties
        # set_property(TARGET GDAL::GDAL
        #     APPEND PROPERTY
        #         IMPORTED_CONFIGURATIONS RELEASE
        # )
        # set_target_properties(GDAL::GDAL PROPERTIES
        #     IMPORTED_LOCATION_RELEASE "${GDAL_LIBRARY_DLL}"
        #     IMPORTED_IMPLIB_RELEASE "${GDAL_LIBRARY}"
        # )
        # # Debug configuration properties
        # set_property(TARGET GDAL::GDAL
        #     APPEND PROPERTY
        #         IMPORTED_CONFIGURATIONS DEBUG
        # )
        # set_target_properties(GDAL::GDAL PROPERTIES
        #     IMPORTED_LOCATION_DEBUG "${GDAL_LIBRARY_DLL}"
        #     IMPORTED_IMPLIB_DEBUG "${GDAL_LIBRARY}"
        # )
        # # common properties
        # set_target_properties(GDAL::GDAL PROPERTIES
        #     INTERFACE_INCLUDE_DIRECTORIES "${GDAL_INCLUDE_DIRS}"
        # )
        # mapping for exotic configurations (doesn't seem to be needed)
        # set_target_properties(GDAL::GDAL PROPERTIES
        #     MAP_IMPORTED_CONFIG_MINSIZEREL Release
        #     MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
        # )
    endif()
endif()

mark_as_advanced(
  GDAL_INCLUDE_DIR
  GDAL_LIBRARY
)
