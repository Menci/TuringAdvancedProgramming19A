gcc -O2 -fPIC -shared yzh-ieee754.c -o ./my.so -w
./checker ./my.so -f calculate -o '/' -r 2000000