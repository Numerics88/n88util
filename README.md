# n88util

Typical random collection of C++ classes and functions that do all the little things
we need all the time.

<!-- [![Build Status](https://dev.azure.com/babesler/n88/_apis/build/status/Numerics88.n88util?branchName=master)](https://dev.azure.com/babesler/n88/_build/latest?definitionId=6&branchName=master) -->
[![Build and upload platform-specific conda packages](https://github.com/Numerics88/n88util/actions/workflows/build-and-upload-conda-packages.yml/badge.svg?branch=master)](https://github.com/Numerics88/n88util/actions/workflows/build-and-upload-conda-packages.yml)
[![Anaconda-Server Badge](https://anaconda.org/numerics88/n88util/badges/version.svg)](https://anaconda.org/numerics88/n88util)
[![Anaconda-Server Badge](https://anaconda.org/numerics88/n88util/badges/platforms.svg)](https://anaconda.org/numerics88/n88util)

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

## CI/CD Pipeline

This repository uses Github Actions to automate the build and deployment process. The pipeline is triggered under the following conditions:

- **Build and Publish to Anaconda.org**
  - Triggered when a new release is published on Github
  - Builts platform-specific Conda packages for macOS x64, macOS arm64, Windows, and Linux
  - Publishes the package to the [Numerics88 channel](https://anaconda.org/numerics88/n88util)

## Notable classes

Refer to the DOxygen-generated documentation for complete details. Here
is a brief description of the most notable classes.

### tuplet

A class for fixed-length tuples (vectors) of simple types.

tuplets are intended to be a replacement for C static arrays,
*e.g.* as an alternative to declaring `float x[3];`

tuplets have no additional storage requirements compared with static arrays
and are equally fast to access.  Unlike C static arrays, the length
can be checked at compile time in places it is used.

Elementary math operations on tuplets are defined, as well as stream IO
operators.

### array

Our take on arrays with multi-dimensional indexing that are
particularly suited to very large amounts of data.

Memory management is done using an ownership model: a single
`array` class owns its data if it allocated it, and the data
will be freed when the owner goes out of scope. arrays can also
reference existing memory or other arrays. In this case it is up to
the user to make sure that the underlying data does not go
out of scope.

You can merrily pass arrays by value without worrying that the
underlying data will be duplicated. The trade-off again is that it
is your responsiblity to not free the data until the function
returns.

Given these design choices, the typical usage is to have a
memory-management scope that creates array objects that
allocate and own their data, and then to call functions
that take arrays as arguments. These passed arrays will automatically
reference the original arrays, and will not own their data.
When you have done manipulating the data, and the functions have
all returned, you can safely let the original arrays go out of scope,
and the allocated memory will be cleaned up.

A particularly handy feature of arrays is that you don't have to
decide at the time of creation whether they will allocate and own
data, or whether they will reference other memory owned by some
other class: you can defer this decision.

### const_array

Identical to array, with the addition that it makes you promise
never to modify the data (cross-your-heart-promise-never-to-use-const_cast).

`const_array` inherits from `array`, so that functions that take `const_array`
can be passed an `array`.

### TrackingAllocator

A very elementary class to help you count up when you allocate
memory, and count down when you free it. The special feature of
this class is that the counter is statically allocated per-thread:
every thread gets its own. Thus you can safely use it in multi-threaded
programs without the speed impact of mutexes or atomic operations.

## Authors and Contributors

n88util is maintained and developed by Numerics88
Solutions Ltd. (http://numerics88.com) The original author is Eric Nodwell (eric.nodwell@numerics88.com).

## Licence

n88util is licensed under a MIT-style open source license. See the file LICENSE.