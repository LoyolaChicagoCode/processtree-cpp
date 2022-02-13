# Required tools

- [CMake](https://cmake.org/)
- [GCC 11](https://gcc.gnu.org/)

# Dependencies

The project uses CMake's `FetchContent` to pull in appropriate versions of these dependencies at the source level during the build process.  
This ensures that the dependencies are link-time compatible with the project itself.

- [GoogleTest](https://google.github.io/googletest)
- [spdLog](https://spdlog.docsforge.com)
- [{fmt}](https://fmt.dev)
- [scnlib](https://scnlib.readthedocs.io)

# Building

```
$ cmake -DCMAKE_C_COMPILER=$(which gcc-11) -DCMAKE_CXX_COMPILER=$(which g++-11) -DCMAKE_BUILD_TYPE=Release -S . -B build
$ cmake --build build
```

These compiler settings should find the installed compilers in a platform-independent way.
The `-DCMAKE_BUILD_TYPE=Release` setting is optional and will turn on compile-time optimization for more realistic performance measurements.

# Running the program

```
$ ps -ef | ./build/bin/processtree
```

# Running the tests

```
$ ./build/bin/processtree_tests
```

# Preliminary insights

- Returning a `process` instance or passing it by reference as an out argument makes no noticeable performance difference.
- Allocating a new `vector<string>` for each input line or converting it into an instance variable of the parser makes no noticeable performance difference.
- `scn::scan_list` is an excellent replacement for `strtok`.
- `scn::getline` is three orders of magnitude slower than `std::getline`: total execution time 20 sec vs. 0.1 sec for 10,000 lines of input.  
- `cin.getline` is quite a bit slower than `std::getline`: total execution time 16 sec vs 8 sec for 2m lines of input.
- spdlog is extremely fast even when using the method calls instead of the compile-time macros.
- stdio is still about three times as fast as the combination of {fmt} and `std::getline`.
- There can be very subtle differences between clang++ and g++.
- It's best to use `FetchContent` for portability across platforms and toolchains and ensure link-time compatibility between a project and its dependencies.  
- The spdlog source has a bundled version of fmt; one needs to compile it against the standalone source of fmt when using both.  
