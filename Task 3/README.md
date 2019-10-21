# Task 3
[Chinese](README.zh.md) | English

Transform simple expressions to pseudo x86-64 assembly code.

The C code to transform is:

1. A function with 3 parameters (all parameters and variables are 64-bit signed integers).
2. 40 local variables (all used).
3. 60 statements of simple assignment expressions, each is in the form of `X = Y op Z`:
    * `X` and `Y` are local variables.
    * `Z` is a local variable, parameter or constant.
    * The operator `op` is `+`, `-`, `*` or `/`.

Your generated C code is:

1. Same form of C function as above.
2. You can use at most 16 local variables (representing 16 registers).
3. You can use an big array (representing memory).
4. All statements should in the form of `X op Y`:
    * If `X` is a local variable (register), `Y` can be a local variable (register), a member of array (memory) or a constant (immediate value).
    * If `X` is a member of array (memory), `Y` can only be a local variable (register) or a constant (immediate value).
    * The operator `op` is `+=`, `-=`, `*=`, `/=` or `=`.

You should optimize your generated code to reduce:

* The times of accessing the array (memory) and local variables (registers).
* The number of different local variables used (registers used).
* The number of different members of array used (memory size used).

You don't need to parse the C code of a function. The origin function is generated randomly by your program itself. Run the original function and your output function with random argument values to check it correctness.

# Deadline
Your Pull Request should be created before October 28, 2019 23:00 (CST).
