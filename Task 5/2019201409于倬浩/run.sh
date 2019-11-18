gcc -fPIC -shared my_qsort.c -o /tmp/my_qsort.so -O3
/tmp/qsort-checker -f Qsort /tmp/my_qsort.so