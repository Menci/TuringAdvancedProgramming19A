# Task 5
[Chinese](README.zh.md) | English

Implementate C standard library function `qsort()`.

According to *Linux Programmer's Manual*, the signature and description of `qsort()` function is:

> ```c
> void qsort(void *base, size_t nmemb, size_t size,
>            int (*compar)(const void *, const void *));
> ```
>
> The `qsort()` function sorts an array with `nmemb` elements of size `size`. The `base` argument points to the start of the array.
>
> The contents of the array are sorted in ascending order according to a comparison function pointed to by `compar`, which is called with two arguments that point to the objects being compared.
>
> The comparison function must return an integer less than, equal to, or greater than zero if the first argument is considered to be respectively less than, equal to, or greater than the second. If two members compare as equal, their order in the sorted array is undefined.

You should implementate a function with the same signature and does the same thing as `qsort()`. Don't let your function be to slow.

Compile your code to a temporary dynamic library with this command:

```bash
$ gcc -O2 -fPIC -shared your_qsort.c -o /tmp/your_qsort.so
```

Then build the `qsort-checker`:

```bash
$ cd "Task 5/qsort-checker"
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

Run the checker with your dynamic library with (remember to replace `your_qsort_function` with your qsort function name):

```bash
$ ./bin/qsort-checker -f your_qsort_function /tmp/your_qsort.so
```

# Grading
* **Code Style**: Perfect, Great, Good, Bad.
* **Correctness**: Yes, No.
* **Speed**: Perfect, Great, Good, Bad.

# Deadline
Your Pull Request should be created before November 18, 2019 23:00 (CST).
