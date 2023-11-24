## Finding GDAL

This module finds GDAL given the following structure of the GDAL package:

``` sh
├── bin
│   └── gdal304.dll
├── include
│   └── gdal
│       ├── arch
│       │   ├── ...
│       ├── atomic
│       │   ├── ...
│       ├── boost
│       │   ├── ...
│       ├── ...
│       ├── gdal.h
│       ├── ...
│       ├── gdal_version.h
│       ├── ...
├── lib
│   └── gdal_i.lib
└── share
    └── gdal
        ├── data
        │   ├── GDALLogoBW.svg
        │   ├── GDALLogoColor.svg
        │   ├── GDALLogoGS.svg
        │   ├── bag_template.xml
        │   ├── cubewerx_extra.wkt
        │   ├── default.rsc
        │   ├── ecw_cs.wkt
        │   ├── eedaconf.json
        │   ├── epsg.wkt
        │   ├── esri_StatePlane_extra.wkt
        │   ├── gdalicon.png
        │   ├── gdalmdiminfo_output.schema.json
        │   ├── gdalvrt.xsd
        │   ├── gml_registry.xml
        │   ├── gmlasconf.xml
        │   ├── gmlasconf.xsd
        │   ├── gt_datum.csv
        │   ├── gt_ellips.csv
        │   ├── header.dxf
        │   ├── inspire_cp_BasicPropertyUnit.gfs
        │   ├── ...
        └── dlls
            ├── freexl.dll
            ├── geos.dll
            ├── geos_c.dll
            ├── iconv-2.dll
            ├── libcrypto-1_1-x64.dll
            ├── libcurl.dll
            ├── libexpat.dll
            ├── libmysql.dll
            ├── libpq.dll
            ├── libssl-1_1-x64.dll
            ├── libxml2.dll
            ├── ogdi.dll
            ├── openjp2.dll
            ├── proj_7_2.dll
            ├── spatialite.dll
            ├── sqlite3.dll
            ├── tiff.dll
            ├── xerces-c_3_2.dll
            └── zlib.dll
```

Configuration:

``` sh
$ cd /path/to/find-gdal
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_PREFIX_PATH="/path/to/gdal" ..
...
-- Found GDAL: /path/to/gdal/include/gdal (found version "3.4.3")
---
-- GDAL_FOUND: TRUE
-- GDAL_FOUND is not a cache variable
---
-- GDAL_VERSION: 3.4.3
-- GDAL_VERSION is not a cache variable
---
-- GDAL_INCLUDE_DIR: /path/to/gdal/include/gdal
-- GDAL_INCLUDE_DIR is a cache variable
---
-- GDAL_INCLUDE_DIRS: /path/to/gdal/include
-- GDAL_INCLUDE_DIRS is not a cache variable
---
-- GDAL_LIBRARY: /path/to/gdal/lib/gdal_i.lib
-- GDAL_LIBRARY is a cache variable
---
-- GDAL_LIBRARIES: /path/to/gdal/lib/gdal_i.lib
-- GDAL_LIBRARIES is not a cache variable
---
-- GDAL_DLLS: /path/to/gdal/bin/gdal304.dll;/path/to/gdal/share/gdal/dlls/freexl.dll;/path/to/gdal/share/gdal/dlls/geos.dll;/path/to/gdal/share/gdal/dlls/geos_c.dll;/path/to/gdal/share/gdal/dlls/iconv-2.dll;/path/to/gdal/share/gdal/dlls/libcrypto-1_1-x64.dll;/path/to/gdal/share/gdal/dlls/libcurl.dll;/path/to/gdal/share/gdal/dlls/libexpat.dll;/path/to/gdal/share/gdal/dlls/libmysql.dll;/path/to/gdal/share/gdal/dlls/libpq.dll;/path/to/gdal/share/gdal/dlls/libssl-1_1-x64.dll;/path/to/gdal/share/gdal/dlls/libxml2.dll;/path/to/gdal/share/gdal/dlls/ogdi.dll;/path/to/gdal/share/gdal/dlls/openjp2.dll;/path/to/gdal/share/gdal/dlls/proj_7_2.dll;/path/to/gdal/share/gdal/dlls/spatialite.dll;/path/to/gdal/share/gdal/dlls/sqlite3.dll;/path/to/gdal/share/gdal/dlls/tiff.dll;/path/to/gdal/share/gdal/dlls/xerces-c_3_2.dll;/path/to/gdal/share/gdal/dlls/zlib.dll
-- GDAL_DLLS is not a cache variable
---
-- GDAL_DATA_DIR: /path/to/gdal/share/gdal/data
-- GDAL_DATA_DIR is not a cache variable
---
...
```
