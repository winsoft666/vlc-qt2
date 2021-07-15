# Building VLC-Qt

## Requirements
VLC-Qt can be built with any common compiler (g++, clang, MSVC, MinGW).
Build files are generated using [CMake](http://www.cmake.org) (3.0.2 or later).


## CMake configuration
Update the [config.cmake](config.cmake) to set the required build configurations.

 * `QT_SDK_DIR`: Qt sdk directory.
 * `LIBVLC_VERSION`: set VLC version you are compiling with to disable unsupported
 	features, should be defined as base 16 integer like `0x020200`, defaults to
 	latest stable VLC version
 * `LIBVLC_LIBRARY`: the path of libvlc.lib
 * `LIBVLCCORE_LIBRARY`: the path of libvlccore.lib
 * `LIBVLC_INCLUDE_DIR`: the directory of libvlc-sdk header files
 * `LIBVLC_BIN_DIR`: the directory of libvlc-sdk bin files
 * `PROJECT_VERSION_MAJOR`: the VLC-Qt library major version, need same as VERSION file
 * `PROJECT_VERSION_MINOR`: the VLC-Qt library minor version, need same as VERSION file
 * `PROJECT_VERSION_PATCH`: the VLC-Qt library patch version, need same as VERSION file
 * `STATIC`: build statically, OFF by default
 * `SYSTEM_QML`: detect and install to system QML location, OFF by default

Extra platform specific flags:
 * `WITH_GLES`: link OpenGL ES v2 on Windows, OFF by default, only needed for
    Qt 5.4 and lower (deprecated)
 * `WITH_HOMEBREW`: let system know you are using Homebrew provided Qt,
    OFF by default, macOS only
 * `WITH_X11`: link with X11, required by some Linux distributions, OFF by default

Building in separate `build` directory is recommended.

## Platform specific instructions
### macOS
You need to prepare VLC libraries and plugins before building. After `cmake` run
`make prepare`, then re-run `cmake`. Build as a normal library or application.

Supported generators are `make` and `ninja`.
Qt in `PATH` and VLC in `/Applications` will be used.

Make example:
```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make prepare
$ cmake ..
$ make -j8
$ make install
```

### Windows
Build as a normal library or application.

Make sure you always use debug libraries with debug build and release libraries
with release build after using it in your project.

Supported generators for MSVC are `nmake`, `jom` and `ninja`;
for MinGW: `mingw32-make` and `ninja`.

Simple example:
```
mkdir build && cd build
cmake .. && cmake --build .
```

### Linux
Install requirements from your distribution's repository.
Supported generators are `make` and `ninja`.

Make example:
```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make -j8
$ make install
```
