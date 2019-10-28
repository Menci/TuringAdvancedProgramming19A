# Task 3
[Chinese](README.zh.md) | English

Transform simple expressions to pseudo x86-64 assembly code.

The C code to transform (*origin function*) is:

1. A function with 3 parameters (all parameters and variables are 64-bit signed integers).
2. 60 local variables (all used).
3. 60 statements of simple assignment expressions, each is in the form of `X = Y op Z`:
    * `X` and `Y` are local variables.
    * `Z` is a local variable, parameter or constant.
    * The operator `op` is `+`, `-`, `*` or `/`.

Your generated pseudo x86-64 assembly code, as the form of C code (*output function*) is:

1. Same form of C function as *origin function*.
2. You can use at most 16 local variables (representing 16 registers).
3. You can use an big array (representing memory).
4. All statements should in the form of `X op Y`:
    * If `X` is a local variable (register), `Y` can be a local variable (register), a member of array (memory) or a constant (immediate value).
    * If `X` is a member of array (memory), `Y` can only be a local variable (register) or a constant (immediate value).
    * The operator `op` is `+=`, `-=`, `*=`, `/=` or `=`.

You should optimize your *output function* to reduce:

* The times of accessing the array (memory) and local variables (registers).
* The number of different local variables used (registers used).
* The number of different members of array used (memory size used).

You should read the *origin function* from test data. The arguments are `a1`, `a2` and `a3`. Local variables are `v1` to `v60`, each line contains an statement, starts with no blank, ends with a `;`. Operators and operands are splitted by a space. For example:

```c
v1 = a1 + 100;
v2 = v1 * 70;
v3 = v1 / v2;
v4 = v3 - v1;
```

To check the correctness of your *output function*, compile the *origin function* and your *output function* and run them with some groups of random arguments and print them values of every expression in *origin function*.

# Deadline
Your Pull Request should be created before October 28, 2019 23:00 (CST).
