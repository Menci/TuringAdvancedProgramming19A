edit: mymalloc.o Splay.so mycode.o Solve.o 
	cc  -DCOMPILETIME -I. -o edit mymalloc.o mycode.o Solve.o ./Splay.so
mymalloc.o: mymalloc.c
	cc -DCOMPILETIME -c mymalloc.c
mycode.o: mycode.c Init2.h
	cc -c mycode.c 
Solve.o: Solve.c Init.h
	cc -c Solve.c
Splay.so: Splay.c
	cc -I. -shared -fPIC -o Splay.so Splay.c

clean:
	rm edit mycode.o Solve.o Splay.so 








