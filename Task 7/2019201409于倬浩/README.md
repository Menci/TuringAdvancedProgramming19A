## Persistent Treap with Runtime Interpositioning

实现了一颗可持久化Treap(无旋Treap), 通过了LOJ #107(源文件`loj_107.c`)。

`Makefile`指定将程序编译为共享库`libptreap.so`并置于当前目录下，并在编译主程序`main.c`时通过`-rpath`指定动态库搜索路径为当前目录。
最终的可执行文件即为`./main`

使用运行时打桩检测内存泄漏问题。`malloc_debug.c`利用当前版本gcc编译器不会在调用`sprintf()`时分配额外缓冲区以输出调试信息，可使用`make malloc_debug.so`得到调试所需动态库，并临时指定环境变量来运行调试：`LD_PRELOAD="./malloc_debug.so" ./main`。

或者执行`./run.sh`调试程序。

惊奇地发现`printf()`存在内存泄漏的feature(bug)。