# Task 6
[Chinese](README.zh.md) | English

1. Solve Josephus problem with double linked list.
2. Manage your project building with `make`.

You need to use C pointers and dynamic memory allocation to implemente your double linked list.

The Josephus problem is:

> There're `n` people sitting in a circle (numbered `1` to `n` clockwise) to play the Josephus game.  
> From the 1st person, `k` persons are skipped clockwise or counterclockwise and then the next person is out.  
> From then on, each turn starts from the next one of the person who is out, skips `k` persons, lets the next person out.  
> Print who is out each turn.

For example, 5 people playing, counterclockwise, 1 person to skip each turn, the answer is: 5, 3, 1, 2, 4.

Your `Makefile` should:

1. Build your program by two steps:
    * Compile each of your C sources to ELF relocatable files in multiple targets (`gcc -c file.c`).
    * Link all ELF relocatable files to an ELF executable file in a target (`gcc file1.o file2.o -o program_name`).
2. Contains the target `clean`, which deletes all files (relocatable and executable) generated during building.

# Grading
* **Code Style**: Perfect, Great, Good, Bad.
* **Correctness**: Yes, No.

# Deadline
Your Pull Request should be created before December 2, 2019 23:00 (CST).
