# n88util

Typical random collection of C++ classes and functions that do all the little things
we need all the time.

Refer to the DOxygen-generated documentation.

## Compiling and linking

n88util requires the following:

  * CMake: www.cmake.org
  * Boost: www.boost.org
  * Google test: https://github.com/google/googletest

To build and run the tests with cmake, on linux or macOS, something like the
following sequence of commands is required:

```sh
mkdir build
cd build
ccmake ..
make
ctest -V
```

On Windows the procedure is a rather different: refer to CMake documentation.


## Authors and Contributors

n88util was developed by Eric Nodwell (eric.nodwell@numerics88.com) at Numerics88
Solutions Ltd.


## Licence

n88util is licensed under a MIT-style open source license. See the file LICENSE.
