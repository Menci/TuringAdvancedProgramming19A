

wsplay: wsplay.o wmain.o mymalloc.o
	gcc -I. -o wsplay wmain.o wsplay.o mymalloc.o
mymalloc.o:
	gcc -DCOMPILETIME -c mymalloc.c
wsplay.o:
	gcc -I. -c splay.c -o wsplay.o
wmain.o:
	gcc -I. -c main.c -o wmain.o

mymalloc.o:
	gcc -DCOMPILETIME -c mymalloc.c


splay: main.o splay.so splay.h
	gcc -o splay main.o ./splay.so

splay.so: splay.c splay.h
	gcc -o splay.so -shared -fpic splay.c

main.o: main.c splay.h
	gcc -c main.c



clean:
	rm -rf *.o *.so wsplay