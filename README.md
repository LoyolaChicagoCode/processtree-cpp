# Requirements

- GoogleTest
- SpdLog

# Building

```
$ cmake -S . -B build
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

# Changing the IO library

To switch between C stdio and iostream, choose between io_stdio.cpp and io_iostream.cpp in the add_executable element for processtree.
