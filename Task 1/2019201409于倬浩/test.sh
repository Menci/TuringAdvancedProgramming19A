gcc -O2 -fPIC -shared main.c -o ./my.so -w
./double-checker ./my.so -f calculate