#!/bin/bash
gcc main.c -o prog1 -O2 -w
gcc convert.c -o prog2 -O2 -w
file="3.txt"
./prog1 $file
./prog2 $file
gcc output.c -o output1 -O2 -w
gcc output2.c -o output2 -O2 -w
./output1
./output2
diff -s result1.txt result2.txt
rm prog1 prog2 output1 output2