gcc -O2 -fPIC -shared fast.c -o ./my.so -w
./double-checker ./my.so -f calculate -r 10000000 -o '/'