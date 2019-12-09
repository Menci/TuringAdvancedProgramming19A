main: treap.h libtreap.so
	gcc -L ./ -Wall -o main main.c -ltreap
libtreap.so: mem.o treap.o
	gcc -shared -o libtreap.so mem.o treap.o
mem.o: mem.c mem.h
	gcc -c -Wall -Werror -fpic mem.c 
treap.o: treap.h treap.c
	gcc -c -Wall -Werror -fpic treap.c
clean:
	rm -f mem.o treap.o libtreap.so main
	