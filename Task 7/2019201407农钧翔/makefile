Splay : structure.o main.o
	cc -o Splay structure.o main.o -g

structure.o : structure.c everything.h
	cc -c structure.c -g
main.o : main.c everything.h
	cc -c main.c -g

clean :
	rm structure.o main.o Splay