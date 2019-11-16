rm /tmp/slowSort.so
gcc slowSort.c -o /tmp/slowSort.so -fPIC -shared -O2 -Wall -Wextra -Wconversion -Wshadow
../qsort-checker/build/bin/qsort-checker -f slowSort /tmp/slowSort.so
