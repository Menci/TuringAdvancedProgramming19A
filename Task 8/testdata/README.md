# Testdata

|Case|`k`|`n`|Data|Property|[`ref`](../ref)|`gzip`|`bzip2`|`xz`|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|1.in|4|300000|`rdtsc.high ^ rdtsc.low`|Highest bits are similar|31.25%|46.17%|47.59%|36.31%
|2.in|8|200000|Random doubles in [0, 1]|One same bit in random-like data|48.72%|71.28%|67.90%|63.77%
|3.in|64|100000|Text with xor `s[i][j] ^ i ^ j`|Reduced number of common patterns|89.29%|64.32%|42.95%|27.71%
|4.in|8|240000|LZMA-compressed `3.in`|Hardly compressible|51.67%|76.79%|74.14%|70.96%
|5.in|8|240000|Normally distributed alphabets|Friendly for 1-bit Huffman|25.13%|55.32%|45.87%|45.35%
|6.in|8|240000|An anime picture in BMP format|Good data locality|49.87%|57.55%|48.89%|45.89%
|7.in|8|240000|An anime picture (transformed to monochrome) in BMP format|Friendly for RLE|10.77%|26.55%|20.52%|20.78%
|8.in|8|240000|[Complete Works of William Shakespeare](http://www.gutenberg.org/cache/epub/100/pg100.txt)|Friendly for LZ77 + Huffman|30.95%|50.70%|39.35%|38.47%

> The [reference implementation](../ref) uses bracket sequence for structure and 1-bit Huffman encoding for data.
