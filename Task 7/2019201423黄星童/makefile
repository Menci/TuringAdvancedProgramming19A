objects = treap dy-treap memory-leak-check.so treap.so treap.o main.o loj-107

treap: main.o treap.o
	gcc main.o treap.o -o treap -O2 -std=c11
main.o: main.c treap.h
	gcc -c main.c -O2 -std=c11
treap.o: treap.c treap.h
	gcc -c treap.c -O2 -std=c11
loj-107: loj-107.c
	gcc loj-107.c -o loj-107 -O2 -std=c11
treap.so: treap.c treap.h
	gcc treap.c -o treap.so -shared -fPIC -O2 -std=c11
memory-leak-check.so: memory-leak-check.c
	gcc memory-leak-check.c -o memory-leak-check.so -shared -fPIC -ldl -O2 -std=c11
shared: main.c treap.so
	gcc main.c ./treap.so -o dy-treap -O2 -std=c11
debug: treap memory-leak-check.so

.PHONY: clean
clean:
	rm -f $(objects)