- [Dependencies](#dependencies)
- [Description](#description)
- [Build & install](#build--install)
  - [Getting sources](#getting-sources)
  - [CMake configuration](#cmake-configuration)
  - [CMake options](#cmake-options)
  - [Compiling](#compiling)
  - [Installing](#installing)
  - [How to link](#how-to-link)
    - [Find package](#find-package)
    - [Link library](#link-library)
- [Owerview](#owerview)
  - [Compiling programs that use the library](#compiling-programs-that-use-the-library)
  - [Resources](#resources)
  - [Bugs and enhancements](#bugs-and-enhancements)

# Dependencies
- [Google test](https://github.com/google/googletest) -- if you need run tests (optional)

# Description
FBI - Fu*king Big Integer

Original version http://mattmccutchen.net/bigint/

You can use this library in a C++ program to do arithmetic on integers of size
limited only by your computer's memory.

# Build & install
## Getting sources
Download from github:
```bash
$ git clone https://github.com/LazyMechanic/fbi
```

## CMake configuration
Create a temporary `build` folder and change your working directory to it:
```bash
$ mkdir build
$ cd build/
```

Make bigint
```bash
$ cmake                                 \
    -DCMAKE_BUILD_TYPE=[Debug|Release]  \
    -G "MinGW Makefiles"                \
    ..
```

For build by `MSVC` you need use Visual Studio Prompt:
```bash
$ cmake                                \
    -DCMAKE_BUILD_TYPE=[Debug|Release] \ 
    -G "NMake Makefiles"               \
    ..
```

## CMake options
```bash
# Google test directory
-DGTest_DIR=path

# Build library tests. OFF by default
-Dfbi_BUILD_TEST=[OFF|ON]

# Build the library as shared. OFF by default
-Dfbi_BUILD_SHARED=[OFF|ON]

# Installation directory for CMake files. "lib/cmake/fbi" by default
-Dfbi_INSTALL_CMAKE_PREFIX=prefix/path

# Installation directory for include files. "include/fbi" by default
-Dfbi_INSTALL_INCLUDE_PREFIX=prefix/path

# Installation directory for binary files. "bin" by default
-Dfbi_INSTALL_BIN_PREFIX=prefix/path

# Installation directory for libraries. "lib" by default
-Dfbi_INSTALL_LIB_PREFIX=prefix/path
```

Generate specific project, for example `Visual Studio solution` generator:
```bash
-G "Visual Studio 16"
```

Set specific compiler, for example `Clang`:
```bash
-DCMAKE_CXX_COMPILER=clang++
```

## Compiling
To compile bigint:
```bash
$ cmake --build .
```

## Installing
To install bigint:
```bash
$ cmake --install . --prefix /path/to/install
```

## How to link
### Find package
Set `-Dfbi_DIR` path to the `fbi-config.cmake` file:
```bash
-Dfbi_DIR=/path/to/directory/with/config
```

Find the package:
```cmake
find_package(fbi CONFIG)

if (NOT ${fbi_FOUND})
    message(FATAL_ERROR "fbi couldn't be located")
endif()
```

### Link library
Link library to target:
```cmake
add_executable(SomeTarget) # Your target
target_link_libraries(
    SomeTarget 
    mech::fbi              # fbi target
```
Include directory will automatically be added to *Sometarget*

# Owerview
C++ Big Integer Library
(see ChangeLog for version)

http://mattmccutchen.net/bigint/

Written and maintained by Matt McCutchen <matt@mattmccutchen.net>

You can use this library in a C++ program to do arithmetic on integers of size
limited only by your computer's memory.  The library provides BigUnsigned and
BigInteger classes that represent nonnegative integers and signed integers,
respectively.  Most of the C++ arithmetic operators are overloaded for these
classes, so big-integer calculations are as easy as:

    #include "BigIntegerLibrary.hh"
    
    BigInteger a = 65536;
    cout << (a * a * a * a * a * a * a * a);
    
    (prints 340282366920938463463374607431768211456)

The code in `sample.cc' demonstrates the most important features of the library.
To get started quickly, read the code and explanations in that file and run it.
If you want more detail or a feature not shown in `sample.cc', consult the
consult the actual header and source files, which are thoroughly commented.

This library emphasizes ease of use and clarity of implementation over speed;
some users will prefer GMP (http://swox.com/gmp/), which is faster.  The code is
intended to be reasonably portable across computers and modern C++ compilers; in
particular, it uses whatever word size the computer provides (32-bit, 64-bit, or
otherwise).

Compiling programs that use the library
---------------------------------------
The library consists of a folder full of C++ header files (`.hh') and source
files (`.cc').  Your own programs should `#include' the necessary header files
and link with the source files.  A makefile that builds the sample program
(`sample.cc') is included; you can adapt it to replace the sample with your own
program.

Alternatively, you can use your own build system or IDE.  In that case, you must
put the library header files where the compiler will find them and arrange to
have your program linked with the library source files; otherwise, you will get
errors about missing header files or "undefined references".  To learn how to do
this, consult the documentation for the build system or IDE; don't bother asking
me.  Adding all the library files to your project will work in many IDEs but may
not be the most desirable approach.

Resources
---------
The library's Web site (above) provides links to released versions, the current
development version, and a mailing list for release announcements, questions,
bug reports, and other discussion of the library.  I would be delighted to hear
from you if you like this library and/or find a good use for it.

Bugs and enhancements
---------------------
The library has been tested by me and others but is by no means bug-free.  If
you find a bug, please report it, whether it comes in the form of compiling
trouble, a mathematically inaccurate result, or a memory-management blooper
(since I use Java, these are altogether too common in my C++).  I generally fix
all reported bugs.  You are also welcome to request enhancements, but I am
unlikely to do substantial amounts of work on enhancements at this point.