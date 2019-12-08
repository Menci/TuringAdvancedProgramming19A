all : main debug_main

main : main.o splay.so
	gcc main.o ./splay.so -o main

splay.so : splay.c
	gcc splay.c -fPIC -shared -o splay.so

main.o : main.c
	gcc -c main.c

debug_main : debug_main.o debug_splay.so my_malloc.o
	gcc -I. my_malloc.o debug_main.o ./debug_splay.so -o debug_main

debug_splay.so : splay.c
	gcc -I. splay.c -fPIC -shared -o debug_splay.so
	
debug_main.o : main.c
	gcc -I. -c main.c -o debug_main.o
	
my_malloc.o : my_malloc.c
	gcc -DCOMPLETETIME -c my_malloc.c

clean :
	rm -r main splay.so main.o\
		debug_main debug_splay.so debug_main.o my_malloc.o
