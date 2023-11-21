## Building plyvel Python wheel

<https://discourse.cmake.org/t/help-with-linking-snappy-to-leveldb-windows-pay-40/9472>

Building `plyvel` Python wheel on Windows with shared libraries.

``` sh
$ cd /e/temp
$ mkdir some && cd $_

$ git clone --depth 1 git@github.com:google/snappy.git
$ git clone --depth 1 git@github.com:google/leveldb.git
$ git clone --depth 1 git@github.com:wbolster/plyvel.git

$ cd ./snappy
$ git rev-parse HEAD
27f34a580be4a3becf5f8c0cba13433f53c21337
$ mkdir build && cd build
$ cmake -G Ninja -DBUILD_SHARED_LIBS=1 -DCMAKE_POSITION_INDEPENDENT_CODE=1 -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=1 -DCMAKE_INSTALL_PREFIX="../install" -DSNAPPY_BUILD_TES
TS=0 -DSNAPPY_BUILD_BENCHMARKS=0 ..
$ cmake --build . --target install

$ cd ../../leveldb
$ git rev-parse HEAD
068d5ee1a3ac40dabd00d211d5013af44be55bea
$ mkdir build && cd build
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=1 -DCMAKE_POSITION_INDEPENDENT_CODE=1 -DLEVELDB_BUILD_TESTS=0 -DLEVELDB_BUILD_BENCHMARKS=0 -DCMAKE_INS
TALL_PREFIX="../install" -DCMAKE_PREFIX_PATH="e:/temp/some/snappy/install" ..
$ cmake --build . --target install

$ cd ../..
$ cp ./snappy/install/bin/snappy.dll ./plyvel/
$ cp ./leveldb/install/bin/leveldb.dll ./plyvel/

$ cd ./plyvel
$ git rev-parse HEAD
fc99b8ebdd1e93ff53138c98bc3f90d786158cad
$ pip install cython
$ scoop install main/make
$ make
$ python setup.py bdist_wheel
$ du -hs ./dist/plyvel-1.5.0-cp311-cp311-win_amd64.whl
100K

$ pip install ./dist/plyvel-1.5.0-cp311-cp311-win_amd64.whl

$ du -hs ./*.dll
260K    ./leveldb.dll
212K    ./snappy.dll

$ python
Python 3.11.2 (tags/v3.11.2:878ead1, Feb  7 2023, 16:38:35) [MSC v.1934 64 bit (AMD64)] on win32

>>> import plyvel
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "e:\temp\some\plyvel\plyvel\__init__.py", line 6, in <module>
    from ._plyvel import (  # noqa
ImportError: DLL load failed while importing _plyvel: The specified module could not be found.

>>> import os
>>> os.add_dll_directory("e:/temp/some/plyvel")
<AddedDllDirectory('e:/temp/some/plyvel')>

>>> import plyvel
>>> db = plyvel.DB('e:/temp/testdb/', create_if_missing=True)
>>> os.listdir("e:/temp/testdb/")
['000003.log', 'CURRENT', 'LOCK', 'LOG', 'MANIFEST-000002']
```

Linking `snappy` from `leveldb` project:

``` cmake
# ...

find_package(Snappy CONFIG REQUIRED)
if(Snappy_FOUND)
  message(STATUS "Found Snappy: ${Snappy_DIR}")
endif()

include(CheckLibraryExists)
check_library_exists(crc32c crc32c_value "" HAVE_CRC32C)
# ugly, but they don't set usual variables for libraries discovery
check_library_exists(snappy snappy_compress "${Snappy_DIR}/../.." HAVE_SNAPPY)
check_library_exists(zstd zstd_compress "" HAVE_ZSTD)
check_library_exists(tcmalloc malloc "" HAVE_TCMALLOC)

message(STATUS "HAVE_SNAPPY: ${HAVE_SNAPPY}")

# ...

if(HAVE_SNAPPY)
  target_link_libraries(leveldb Snappy::snappy)
endif(HAVE_SNAPPY)

# ...
```

Also `setup.py` modifications:

``` py
extra_compile_args = ['-Wall', '-g', '-x', 'c++', '-std=c++11', '-Ie:/temp/some/leveldb/install/include']

if platform.system() == 'Darwin':
    extra_compile_args += ['-stdlib=libc++']

ext_modules = [
    Extension(
        'plyvel._plyvel',
        sources=['plyvel/_plyvel.cpp', 'plyvel/comparator.cpp'],
        # alternative to setting -I flag
        #include_dirs=["e:/temp/some/leveldb/install/include"],
        library_dirs=["e:/temp/some/leveldb/install/lib"],
        libraries=['leveldb'],
        extra_compile_args=extra_compile_args
    )
]
```
