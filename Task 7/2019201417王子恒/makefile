all : main debug_main

main : main.o splay.so splay.h
	gcc main.o ./splay.so -o main

splay.so : splay.c splay.h
	gcc splay.c -fPIC -shared -o splay.so

main.o : main.c splay.h
	gcc -c main.c

debug_main : debug_main.o debug_splay.so my_malloc.o splay.h malloc.h
	gcc -I. my_malloc.o debug_main.o ./debug_splay.so -o debug_main

debug_splay.so : splay.c splay.h malloc.h
	gcc -I. splay.c -fPIC -shared -o debug_splay.so
	
debug_main.o : main.c splay.h malloc.h
	gcc -I. -c main.c -o debug_main.o
	
my_malloc.o : my_malloc.c malloc.h
	gcc -DCOMPLETETIME -c my_malloc.c

clean :
	rm -r main splay.so main.o\
		debug_main debug_splay.so debug_main.o my_malloc.o
