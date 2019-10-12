# Task 2
[Chinese](README.zh.md) | English

Expression evaluator with floating-point support.

* Use your implementation of IEEE 754 floating-point number in Task 1.
* Implementate input / output by yourself.
* Evaluate a expression with `+`, `-`, `*`, `/`, `()` and **decimal numbers**. Spaces between numbers and operators should be ignored if exists.
* Read the expression from `stdin` and write the value to `stdout` (both are one per line).
* It's guaranteed that the input expressions is valid.

A **decimal number** contains:

* A optional leading `+` or `-` sign.
* A `0`, or one or more decimal digits without leading zero, as the integer part.
* A optional decimal point `.`.
* (If decimal point `.` exists) one or more decimal digits without trailing zero, as the decimal part.

Comparing to the answer by Python, one of the relative error or the absolute error of your output should be no more than `1e-8`.

When the result is infinity or not-a-number, print it as `inf` (or `+inf`), `-inf`, `nan` (or `+nan`), `-nan`.

You'll need Python 3 to run `evaluator-checker`.

```bash
$ cd "Task 2"
$ python3 evaluator-checker /path/to/your/compiled/program/a.out
```

# Deadline
Your Pull Request should be created before October 21, 2019 23:00 (CST).
