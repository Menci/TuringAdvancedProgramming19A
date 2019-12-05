# Task 6

首先构建项目：

```shell
make Release
```

这会在当前目录下新建一个 `tmp/` 文件夹用于存储所有 ELF 可重定位文件，并将这些文件链接，在当前目录下生成可执行文件 `Josephus`。

然后执行

```shell
./Josephus -h
```

或

```shell
./Josephus --help
```

获取相关参数的传递方式。