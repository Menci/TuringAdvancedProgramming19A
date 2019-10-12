rm /tmp/main.so
gcc -O2 -fPIC -shared main.c -o /tmp/main.so 
../double-checker/build/bin/double-checker -f checker /tmp/main.so
