## qsort


学习了一下C++ STL的sort()算法，写了一个IntroSort。

实现函数： `void Qsort((void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)))`

正常情况下，将区间三个端点中值设为Pivot，运行快速排序。在递归层数较深的时候会调用堆排序（不过经测试这个优化在当前数据中效果并不是很好），在区间较短的时候调用插入排序，减少递归调用。

Update : 
写完以后发现全世界都上了多线程，又不想重新写归并排序，于是在原基础上加了一个多线程版本。

直接将原序列分块，根据机器逻辑核心数量确定线程数，多线程调用Qsort()，最后一次合并，时间复杂度$O(\frac{nlogn}{t} + nt)$。对于大多数数据实测可以快1倍（i7-6700HQ Ubuntu19.04 64位）。但是WSL Ubuntu 18.04下，大结构体排序效果不好。

`*_pth`即为多线程相关的文件。

测试方法：可以先使用`build.sh`编译checker，之后`run_pth.sh`编译运行程序。