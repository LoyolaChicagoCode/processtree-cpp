# Requirements

- [GoogleTest](https://google.github.io/googletest)
- [spdLog](https://spdlog.docsforge.com)
- [{fmt}](https://fmt.dev)
- [scnlib](https://scnlib.readthedocs.io)

# Building

```
$ cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
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
- `scn::getline` is three orders of magnitude slower than `std::getline`: 20 sec vs. 0.1 sec for 10,000 lines of input.  
- spdlog extremely fast even when using the method calls instead of the compile-time macros.
- stdio is still three times faster than the combination of {fmt} and 