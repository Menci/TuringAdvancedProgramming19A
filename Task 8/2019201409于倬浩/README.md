# Strange Operations On A Binary Tree

使用方法：make传入K的值，运行时传入输入文件名。
`./converter [0/1] [filename]`, 0表示压缩，1表示解压。


```
make K=3
```

对应的第一组测试数据输出：
```
$ ./converter 0 1.in
[input.bin] size = 3600000 Bytes, K = 3, n = 300000
[Compress] zipped.bin = 1200000 Bytes
$ ./converter 1 1.in
[input.bin] size = 3600000 Bytes, K = 3, n = 300000
[Decompress] Check Passed!
```