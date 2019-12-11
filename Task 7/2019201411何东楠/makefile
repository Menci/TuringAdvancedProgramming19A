main : main.o splay.so my_malloc.o
	gcc -o main main.o ./splay.so my_malloc.o
main.o : main.c splay.c splay.h malloc.h
	gcc -c main.c 
splay.so : splay.c splay.h
	gcc -I. -shared -fPIC -o splay.so splay.c
my_malloc.o : malloc.h my_malloc.c
	gcc -c my_malloc.c
clean :
	rm main *.o *.so
