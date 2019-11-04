# Task 1
中文 | [英文](README.md)

实现 IEEE 754 64 位浮点数运算。

* 需要支持 `+`, `-`, `*` 和 `/`。
* 你的数字的结构需要与 C 语言的 `double` 类型完全相同。
* 边界情况需要正确处理（如非规约数和无穷）。
* 不要太慢。

你的实现将被 `double-checker` 检查。它会调用你代码中的这样一个计算函数（你的代码将被编译为[动态库](https://zh.wikipedia.org/wiki/函式庫)）：

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

你可以更改这个函数的名称，但需要保证它的参数与返回值类型与上述相同。注意你的 `main()` 函数***不会***被调用，所以你需要在这个函数第一次被调用时执行你的初始化工作（如果有）。这个函数将在运行测试前被调用一次，以保证你的初始化工作不会被计入运行时间。

实现计算函数后，将你的代码编译为临时的动态库：

```bash
$ gcc -O2 -fPIC -shared your_double.c -o /tmp/your_double.so
```

然后，编译 `double-checker`（只需编译一次）：

```bash
$ cd "Task 1/double-checker"
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

使用你的动态库运行 `double-checker`（记住将 `your_calculate_function` 替换为你的函数名）：

```bash
$ ./bin/double-checker -f your_calculate_function /tmp/your_double.so
```

测试结果与你的代码本身都会影响你的成绩。

玩得开心！

# 评分
* **代码风格**：最好、好、一般、差。
* **正确性**：最好、好、一般、差。
* **速度**：最好、好、一般、差。
* **可扩展性**：高、低。

# 截止日期
你的 Pull Request 需要在 2019 年 10 月 12 日 23:00（中国标准时间）前被创建。
