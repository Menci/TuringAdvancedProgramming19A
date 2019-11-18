## qsort


学习了一下C++ STL的sort()算法，写了一个IntroSort。

实现函数： `void Qsort((void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)))`

正常情况下，将区间三个端点中值设为Pivot，运行快速排序。在递归层数较深的时候会调用堆排序（不过经测试这个优化在当前数据中效果并不是很好），在区间较短的时候调用插入排序，减少递归调用。

PS: 全世界都上了多线程，于是加了一个多线程版本，根据机器逻辑核心数量确定线程数，`*_pth`即为多线程相关的文件。