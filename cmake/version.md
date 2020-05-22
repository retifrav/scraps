## Embed version information

https://docs.microsoft.com/en-us/windows/win32/menurc/versioninfo-resource

`CMakeLists.txt`:

```
set(PRODUCT_NUMBER 1)
set(PRODUCT_VERSION 0)
set(BUILD_NUMBER 15)
set(ORIGINAL_FILE_NAME ${target})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
)

add_executable( ${target} ${sources} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
```

`version.rc.in`:

```
#define VER_FILEVERSION        @PRODUCT_NUMBER@,@PRODUCT_VERSION@,@BUILD_NUMBER@
#define VER_PRODUCTVERSION     @PRODUCT_NUMBER@,@PRODUCT_VERSION@,@BUILD_NUMBER@

#define VER_COMPANYNAME_STR     "Some company"
#define VER_FILEDESCRIPTION_STR "Your application name"
#define VER_FILEVERSION_STR     "@PRODUCT_NUMBER@.@PRODUCT_VERSION@.@BUILD_NUMBER@"
#define VER_INTERNALNAME_STR    "Your application name"
#define VER_LEGALCOPYRIGHT_STR  "2020, Some company or other legal name"
#define VER_LEGALTRADEMARKS1_STR ""
#define VER_LEGALTRADEMARKS2_STR ""
#define VER_ORIGINALFILENAME_STR "@ORIGINAL_FILE_NAME@.exe"
#define VER_PRODUCTNAME_STR      "Your application name"
#define VER_PRODUCTVERSION_STR   "@PRODUCT_NUMBER@.@PRODUCT_VERSION@.@BUILD_NUMBER@"

1 VERSIONINFO
FILEVERSION     VER_FILEVERSION
PRODUCTVERSION  VER_PRODUCTVERSION
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "040904E4"
        BEGIN
            VALUE "CompanyName",      VER_COMPANYNAME_STR
            VALUE "FileDescription",  VER_FILEDESCRIPTION_STR
            VALUE "FileVersion",      VER_FILEVERSION_STR
            VALUE "InternalName",     VER_INTERNALNAME_STR
            VALUE "LegalCopyright",   VER_LEGALCOPYRIGHT_STR
            VALUE "LegalTrademarks1", VER_LEGALTRADEMARKS1_STR
            VALUE "LegalTrademarks2", VER_LEGALTRADEMARKS2_STR
            VALUE "OriginalFilename", VER_ORIGINALFILENAME_STR
            VALUE "ProductName",      VER_PRODUCTNAME_STR
            VALUE "ProductVersion",   VER_PRODUCTVERSION_STR
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x0409, 1252
    END
END
```
