# Loading dynamic libraries on Mac OS

<https://gitlab.kitware.com/cmake/cmake/-/issues/21854>

If a dynamic/SHARED (*`.dylib`*) library depends on other libraries which are also dynamic/SHARED, its signature will get messed up by `install_name_tool` on building that library project (*not this project*).

To avoid/workaround this, that library's project should have:

``` cmake
set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")
```

And this project here is just for testing the loading of the resulting library. It should print some address, if loading the library was successful, otherwise it will print `0x0` (*`NULL`*).

Note that aside from messed up signature the `.dylib` file can also have [quarantine attribute](https://github.com/retifrav/scraps/blob/master/_macos/index.md#removing-all-attributes) assigned to it, which will also result in `0x0`/`NULL` on trying to load it.

One other note is that if the library has dependencies which are also dynamic/SHARED libraries, they need to be loaded first.
