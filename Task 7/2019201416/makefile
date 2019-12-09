main: splay.so splay.h
	gcc main.c ./splay.so -o main
debug: desplay.so splay.h mymalloc.o malloc.h main.c
	gcc -I. -o debug main.c mymalloc.o ./desplay.so
splay.so: splay.c
	gcc splay.c -fPIC -shared -o splay.so
desplay.so: splay.c
	gcc splay.c -I. -fPIC -shared -o desplay.so
mymalloc.o: mymalloc.c
	gcc -DCOMPILETIME -c mymalloc.c
clean:
	rm -rf main
	rm -rf debug
	rm -rf *.o
	rm -rf *.so
