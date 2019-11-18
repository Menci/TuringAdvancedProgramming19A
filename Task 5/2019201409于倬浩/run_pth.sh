gcc -fPIC -shared my_qsort_pth.c -o /tmp/my_qsort.so -Ofast -lpthread
/tmp/qsort-checker -f Qsort /tmp/my_qsort.so