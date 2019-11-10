# Task 5
中文 | [英文](README.md)

实现 C 标准库函数 `qsort()`。

根据 *Linux Programmer's Manual*，`qsort()` 函数的签名和描述为：

> ```c
> void qsort(void *base, size_t nmemb, size_t size,
>            int (*compar)(const void *, const void *));
> ```
>
> `qsort()` 函数对一个有 `nmemb` 个大小为 `size` 的元素的数组进行排序。`base` 参数指向数组的开始。
>
> 数组的内容被以升序排序，通过比较函数 `compar` 的比较，该函数会被以两个指向将被排序的对象的指针调用。
>
> 比较函数必须返回一个小于、等于或大于零的整数如果第一个参数被认为严格小于、等于或严格大于第二个。如果两个成员相等，他们在排好序的数组中的相对位置是未定义的。

你需要实现一个与 `qsort()` 具有相同的签名和功能的函数，不要太慢。

将你的代码编译为临时的动态库：

```bash
$ gcc -O2 -fPIC -shared your_qsort.c -o /tmp/your_qsort.so
```

然后，编译 `double-checker`：

```bash
$ cd "Task 5/qsort-checker"
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

使用你的动态库运行 `qsort-checker`（记住将 `your_qsort_function` 替换为你的函数名）：

```bash
$ ./bin/qsort-checker -f your_qsort_function /tmp/your_qsort.so
```

# 评分
* **代码风格**：最好、好、一般、差。
* **正确性**：是、否。
* **速度**：最好、好、一般、差。

# 截止日期
你的 Pull Request 需要在 2019 年 11 月 18 日 23:00（中国标准时间）前被创建。
