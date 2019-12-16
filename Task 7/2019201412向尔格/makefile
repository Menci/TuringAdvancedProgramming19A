Final : function.o splay.o wrap.o
	cc -o Final function.o splay.o wrap.o

wrap.o : malloc.h wrap.c
	cc -Dcompile -c wrap.c

splay.o : splay.c function.h wrap.c
	cc -I. -c splay.c

function.o : function.c function.h
	cc -I. -c function.c

clean :
	rm splay.o function.o Final wrap.o
