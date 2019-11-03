# Task 4
[Chinese](README.zh.md) | English

Transform simple expressions to real x86-64 assembly code.

Just like in Task 3:

> The C code to transform (*origin function*) is:
>
> 1. A function with 3 parameters (all parameters and variables are 64-bit signed integers).
> 2. 60 local variables (all used).
> 3. 60 statements of simple assignment expressions, each is in the form of `X = Y op Z`:
>     * `X` and `Y` are local variables.
>     * `Z` is a local variable, parameter or constant.
>     * The operator `op` is `+`, `-`, `*` or `/`.
>
> You should read the *origin function* from test data. The arguments are `a1`, `a2` and `a3`. Local variables are `x1` to `x60`, each line contains an statement, starts with no blank, ends with a `;`. Operators and operands are splitted by a space. For example:
>
> ```c
> x1 = a1 + 100;
> x2 = x1 * 70;
> x3 = x1 / x2;
> x4 = x3 - x1;
> ```

Now we modify the function to let it return the xor-sum of all variables (i.e. `x1 ^ x2 ^ ... ^ x60`). Instead of C code, you should generate *real* x86-64 assembly code:

1. The three arguments will be passed through `rdi`, `rsi` and `rdx`.
2. The return value should be passed through `rax`.
3. You must preserve the value of registers below, [according to Linux x86-64 ABI](http://6.s081.scripts.mit.edu/sp18/x86-64-architecture-guide.html):
    * `rbx`
    * `rsp`
    * `rbp`
    * `r12`
    * `r13`
    * `r14`
    * `r15`

You should optimize your *output assembly code* to reduce:

* The times of accessing the memory and registers.
* The number of different registers used.
* The memory size used.

You should print AT&T assembly code without declarations, like:

```asm
movq $0, %rax
ret
```

To check the correctness of your output assembly code, run the checker:

```bash
$ cd "Task 4"
$ python3 assembly-checker /path/to/your/compiled/program/a.out
```

# Deadline
Your Pull Request should be created before November 4, 2019 23:00 (CST).
