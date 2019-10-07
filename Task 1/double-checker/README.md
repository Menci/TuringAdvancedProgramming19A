# Double Checker
Checker for your double calculating implementation, used in Task 1.

Please refer to Task 1's [README](../README.md).

# Build
```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

# Usage
```
$ ./bin/double-checker --help
Usage: ./build/bin/double-checker [-o <operators>] [-r <number>] [-s <number>] [-f <name>] <dynamic_library>
       ./build/bin/double-checker -?

Checker for double calculating.

Positional arguments:
  <dynamic_library>                     Your double implementation dynamic library. (e.g. double.so)

Options:
  -?, --help                            Show this help message and exit.
  -o, --operators=<operators>           The operators to check, in a string without any delimiter. (Default: +-*/)
  -r, --check-rounds=<number>           The number of rounds to check, per operator. (Default: 2000000)
  -s, --seed=<number>                   The random seed for RNG. Zero for a random value. (Default: 0)
  -f, --function-name=<name>            The function name exported by your dynamic library. (Default: double_checker_calc)
```
