# Selene

## Building the library

### General

**Selene** uses [CMake](https://cmake.org/) for building.

Follow the usual instructions for CMake projects on UNIX-like systems.
First, clone the project and create a `build` directory (or use another name).

    git clone https://github.com/kmhofmann/selene.git
    cd selene
    mkdir build && cd build

Then call `cmake` and build the project.

    cmake -DCMAKE_BUILD_TYPE=Release -DSELENE_BUILD_ALL=ON ..
    cmake --build .      # or just 'make', if GNU Make is the generator

Additional options may be passed to the respective build tool, e.g. `cmake --build . -- -j8`.

On Windows, the CMake command might look similar to the following, in order to generate Visual Studio 2017 project
files for a 64-bit build (see below for more info on [vcpkg](https://github.com/Microsoft/vcpkg)):

    cmake -G "Visual Studio 15 2017 Win64" -T "host=x64" \
        -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>\scripts\buildsystems\vcpkg.cmake \
        -DSELENE_BUILD_ALL=ON \
        ..

The setting `-DSELENE_BUILD_ALL=ON` enables building the tests, examples, and benchmarks.
Omit this parameter if this is not desired, and see below for the individual CMake variables that can be set for
more fine-grained control.

#### Static vs. shared libraries

The default CMake settings will build a set of static libraries.

If you want to build shared libraries instead, add `-DBUILD_SHARED_LIBS=ON` to the `cmake` command.

### Usage/Installation

* The easiest option is to use the library as a submodule within your project.
  No actual installation is needed then, and Selene will be built from source together with your project.
  Integrating the library into own CMake projects can be as easy as:

      add_subdirectory(selene)  # assuming the library is cloned as submodule in a directory named 'selene'
      # ...
      target_link_libraries(<target_name> selene::selene)

  Advantages of this approach are greatly decreased risk of inconsistent dependencies (in case you upgrade libraries),
  and IDEs more easily picking up the Selene source code (as opposed to, say, just the installed headers).
  This can be particularly useful when developing on Selene itself.

* Alternatively, you can install Selene (e.g. using `make install`) and then declare as dependency in a CMake project
  as follows:

      find_package(selene)
      # ...
      target_link_libraries(<target_name> selene::selene)

  To provide a custom installation location, add `-DCMAKE_INSTALL_PREFIX=<your_custom_location>` to the CMake invocation.
  The default is an intrusive, system-wide `/usr/local` on UNIX-like systems; it is recommended to change this.

The CMake invocation also adds a reference to the build tree location to the user-level CMake cache.
This means that the `find_package()` call can also work without installation, and will find then find the build tree
itself. 

### Dependencies

**Selene** uses the following (optional) third-party dependencies for implementing some of its functionality:

  - [libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo) or [libjpeg](http://www.ijg.org/):
    - Optional, but recommended.
    - Required for the JPEG reading and writing API.
    - `libjpeg-turbo` is the preferred version of the library.
  - [libpng](http://www.libpng.org/pub/png/libpng.html):
    - Optional, but recommended.
    - Required for the PNG reading and writing API.
  - [OpenCV](https://opencv.org/):
    - Optional, if needed.
    - For OpenCV interoperability (e.g. copying or wrapping image data).
    - **Not required** for building the library, since OpenCV interoperability is implemented in a header file only.

The presence (or lack of) these dependencies should be detected automatically by CMake.

If [libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo) is present on the system (as opposed to IJG *libjpeg*),
some of its additional capabilities (such as partial JPEG image decoding and extended color spaces) will be taken into
account and are enabled by the API.

[OpenCV](https://opencv.org/) is only needed for converting between `sln::Image<T>` and OpenCV's `cv::Mat` structure, if
so desired.
It is by no means a requirement for using **Selene**.

To point CMake to custom library installation locations, set the `CMAKE_PREFIX_PATH` environment variable accordingly.
For example, `export CMAKE_PREFIX_PATH=$HOME/local/libjpeg-turbo:$HOME/local/libpng`. 

The use of [Conan](https://conan.io/) or [vcpkg](https://github.com/Microsoft/vcpkg) as dependency package managers is
also supported; see below.

### Building and running tests, examples, and benchmarks

#### Everything

    -DSELENE_BUILD_ALL=ON
    
This is a convenience option to enable building tests, examples, and benchmarks.
More fine-grained control can be achieved by the options right below.

#### Tests

    -DSELENE_BUILD_TESTS=ON

Building the tests is disabled by default, and can be enabled by adding `-DSELENE_BUILD_TESTS=ON` to the `cmake`
command.

The test suite depends on [Catch2](https://github.com/catchorg/Catch2) and [Boost.Filesystem](http://www.boost.org/) for
building the tests.
The former is bundled as a Git submodule and will be automatically cloned during execution of the `cmake` command.
The latter is automatically searched for by a CMake `find_package` command; its presence is required to build the tests.

The test suite can be run by executing `./test/selene_tests` from the `build` directory. `./test/selene_tests -h` lists
available options. See the [Catch2 documentation](https://github.com/catchorg/Catch2/blob/master/docs/command-line.md)
for more information.

#### Examples

    -DSELENE_BUILD_EXAMPLES=ON

The repository also contains commented examples which can be optionally compiled.
This can be enabled by adding `-DSELENE_BUILD_TESTS=ON` to the `cmake` command.
The examples can then be found in the `./examples/` folder in the build directory.

#### Benchmarks

    -DSELENE_BUILD_BENCHMARKS=ON

A few micro-benchmarks can be optionally compiled by adding `-DSELENE_BUILD_BENCHMARKS=ON` to the `cmake` command line.
The code for these can be found in the `./benchmark/` folder, and depends on Google's
[benchmark](https://github.com/google/benchmark) library to be installed.

#### Specifying the data path

In case some tests or examples are failing because auxiliary data files can not be found automatically, specify the path
to the `data` directory inside the `selene/` folder manually: `SELENE_DATA_PATH=../data ./test/selene_tests` (or
similar).

### Installing dependencies

The following are recommendations for installation of dependencies on various platforms.
Of course any dependency can alternatively also be built and installed from source, as long as it can be found by the
CMake `find_package` command.

#### Linux

On Debian-like systems (e.g. Ubuntu), you should be able to use `apt-get` as follows:

    # Dependencies for building the library
    apt-get install libjpeg-turbo8-dev libpng-dev
    
    # Dependencies for building tests & examples
    apt-get install libopencv-dev libboost-filesystem-dev
    
    # Note: There is no pre-built google-benchmark package in e.g. Ubuntu.
    #       Install from source instead, or use vcpkg.

#### MacOS

Install [Homebrew](https://brew.sh/) to build and install the dependencies as follows:

    # Dependencies for building the library
    brew install libjpeg-turbo libpng
    
    # Dependencies for building tests, examples, benchmarks
    brew install opencv3 boost google-benchmark

#### Windows

By far the easiest way is to install and then use the [vcpkg](https://github.com/Microsoft/vcpkg) package manager:

    .\vcpkg.exe install libjpeg-turbo
    .\vcpkg.exe install libpng
    
    .\vcpkg.exe install opencv     # only for tests
    .\vcpkg.exe install boost      # only for tests
    .\vcpkg.exe install benchmark  # only for benchmarks

Set the system environment variable `VCPKG_DEFAULT_TRIPLET=x64-windows` before installing the above packages to install
the 64-bit compiled versions instead of the 32-bit ones.

#### Using Conan

**Selene** supports optional use of [Conan](https://conan.io/) as a dependency manager on all supported platforms.
(It is also possible to install [a release of Selene itself](https://bintray.com/kmhofmann/conan-repo/selene%3Aselene) using Conan).
See the full [Conan documentation](https://docs.conan.io/) for more information on how to use Conan.

Currently, `libjpeg-turbo`, `libpng` and `Boost.Filesystem` (for the tests) can be built using Conan.
There are no stable, cross-platform Conan recipes available yet for `OpenCV` or Google's `benchmark` library.

To use Conan, first install it, e.g. with `pip`:

    pip install --user --upgrade conan

(On MacOS, you might prefer an installation with `brew` instead.)

The `conan-center` remote should be present with a default installation.
This can be checked by the `conan remote list` command.
If so, no further action is necessary; if not, add it with:

    conan remote add conan-center https://conan.bintray.com

From your (clean) build directory, call Conan before the CMake invocation (as follows, or similar):

    conan install .. --build missing
    
This will build the supported dependencies.
This step can take a while during its first call, but for future builds, all outputs should be cached.

By default, Conan builds static libraries.
If you want Conan to build shared libraries instead, add `-o *:shared=True` to the above Conan call.

Now you can invoke CMake as usual (see above), and it should find the respective Conan builds of the supported
dependencies.
