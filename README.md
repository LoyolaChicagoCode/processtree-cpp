# Requirements

- [GoogleTest](https://google.github.io/googletest)
- [spdLog](https://spdlog.docsforge.com)
- [{fmt}](https://fmt.dev)
- [scnlib](https://scnlib.readthedocs.io)

# Building

```
$ cmake -DCMAKE_C_COMPILER=$(which gcc-11) -DCMAKE_CXX_COMPILER=$(which g++-11) -DCMAKE_BUILD_TYPE=Release -S . -B build
$ cmake --build build
```

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
- It's best to use `FetchContent` for portability across platforms and toolchains. 
