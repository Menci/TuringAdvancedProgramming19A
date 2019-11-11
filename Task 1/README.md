# Task 1
[Chinese](README.zh.md) | English

Implementate IEEE 754 64-bit floating point calculation.

* `+`, `-`, `*` and `/` should be supported.
* Your number's structure should be exactly the same as C's `double`.
* Edge cases such as denormalized or infinity should be handled correctly.
* Don't be toooooooooooooooooo slow.

Your implementation will be checked by the `double-checker`. It calls a function in your code, which need to be compiled to a [dynamic library](https://en.wikipedia.org/wiki/Library_(computing)). The function should be like this:

```c
#include <stdint.h>  // uint64_t
#include <stdbool.h> // bool

uint64_t your_calculate_function(uint64_t a, uint64_t b, char op) {
    static bool initialized = false;
    if (!initialized) {
        do_your_initialize();
        initialized = true;
    }

    your_double_type x = convert_to_your_double_type(a),
                     y = convert_to_your_double_type(b),
                     result;
    if (op == '+') result = your_add(x, y);
    // ...
    return convert_from_your_double_type(result);
}
```

You can change the function name but your function must have the same types of arguments and return value as above. Notice that your `main()` function will ***NOT*** be called, so you should do your initialize work (if exists) when this function is called the first time. It WILL be called once before running check to ensure your initialize work won't add to your run time.

After writing your calculate function, compile your code to a temporary dynamic library with this command:

```bash
$ gcc -O2 -fPIC -shared your_double.c -o /tmp/your_double.so
```

Then build the `double-checker` (you just need to build it only once):

```bash
$ cd "Task 1/double-checker"
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

Run the checker with your dynamic library with (remember to replace `your_calculate_function` with your calculate function name):

```bash
$ ./bin/double-checker -f your_calculate_function /tmp/your_double.so
```

Both the result of checking and your code itself influences your grade.

Have fun!

# Grading
* **Code Style**: Perfect, Great, Good, Bad.
* **Correctness**: Perfect, Great, Good, Bad.
* **Speed**: Perfect, Great, Good, Bad.
* **Scalability**: High, Low.

# Deadline
Your Pull Request should be created before October 12, 2019 23:00 (CST).
