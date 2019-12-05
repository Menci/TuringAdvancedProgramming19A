Josephus:Josephus.o link.o link.h
	gcc -o Josephus Josephus.o link.o
Josephus.o:Josephus.c link.h
	gcc -c Josephus.c
link.o:link.c link.h
	gcc -c link.c
clean:
	rm link.o Josephus.o Josephus

