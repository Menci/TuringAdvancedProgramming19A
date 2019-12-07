Tree: splay.so main.o
	gcc -o Tree main.o ./splay.so
splay.so: 
	gcc -shared -fpic -o splay.so splay.c
main.o:
	gcc -o main.o -c main.c
wTree: wsplay.so wmain.o mymalloc.o
	gcc -I. -o wTree wmain.o ./wsplay.so mymalloc.o
mymalloc.o:
	gcc -DCOMPILETIME -c mymalloc.c
wsplay.so:
	gcc -I. -shared -fpic -o wsplay.so splay.c
wmain.o:
	gcc -I. -c main.c -o wmain.o
clean:
	rm -rf main.o splay.so Tree wTree mymalloc.o wsplay.so wmain.o
