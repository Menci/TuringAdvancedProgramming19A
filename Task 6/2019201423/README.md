# Task 6
This task is to print the dequeue sequence of Josephus problem using the doubly circular linked list.

## How to Use
You need to use `make` to compile my source code first.
```bash
$ cd "./Task 6/2019201423黄星童"
$ make
```

Then run the executable file.
```bash
$ ./Josephus arg1 arg2 arg3
```
The `arg1` represents `n` in the problem and it should be a positive `int`.
The `arg2` represents `k` in the problem and it should be a positive `int`.
The `arg3` is either `-1` or `1`. `-1` is counterclockwise and `1` is clockwise.

If your input format is wrong, you will see the following prompt in the terminal.
```
Input Format Error
Please read "README.md".
```

After use, you can use this command to delete all the object files and the executable file.
```bash
$ make clean
```