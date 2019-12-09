# Task 7
A FHQ-Treap by C.

## How to Use
Normally, you should use `make` to compile the code to an executable file.
```bash
$ cd "./Task 7/2019201423黄星童"
$ make
$ ./treap
```

If you want to get a program that can be accepted by [LibreOJ-107](https://loj.ac/problem/107), you should submit `loj-107.c`. You can get a local executable file called `loj-107` by typing this command. My submission is [here](https://loj.ac/submission/693069).
```bash
$ make loj-107
```

If you want to compile my code to a shared object file, you can use this command. It will generate a shared object file called `treap.so` and an executable file called `dy-treap`.
```bash
$ make shared
$ ./dy-treap
```

If you want to check for memory leaks, you can use these commands.
```bash
$ make debug
$ LD_PRELOAD="./memory-leak-check.so" ./treap
```