# Algorithm

括号序列 + LZW

# Output

zipped.bin是括号序列对input.bin压缩后的文件

zippeded.bin是lzw对zipped.bin压缩后的文件

unzipped.bin是lzw对zippeded.bin解压后的文件

unzipped.bin和stdout.bin是对树进行先序遍历后得到的的文件，用于检查解压正确性

# Testdata

|Case|`LZW+括号序列`|`括号序列`|
|:-:|:-:|:-:|
|1.in|45.02%|34.99%|
|2.in|72.73%|51.57%|
|3.in|59.40%|89.22%|
|4.in|78.88%|51.57%|
|5.in|38.37%|51.57%|
|6.in|56.99%|51.57%|
|7.in|9.36%|51.57%|
|8.in|35.17%|51.57%|

