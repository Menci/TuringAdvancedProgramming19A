edit:  Splay.so mycode.o Solve.o 
	cc -o edit  mycode.o Solve.o ./Splay.so
mymalloc.o: mymalloc.c
	cc -DLINKTIME -c mymalloc.c
mycode.o: mycode.c Init2.h
	cc -c mycode.c 
Solve.o: Solve.c Init.h
	cc -c Solve.c
Splay.so: Splay.c mymalloc.o
	cc -DLINKTIME -Wl,--wrap,malloc -Wl,--wrap,free -shared -fPIC -o Splay.so Splay.c mymalloc.o

clean:
	rm edit mycode.o Solve.o Splay.so mymalloc.o








