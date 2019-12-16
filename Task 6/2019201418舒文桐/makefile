edit: main.o In.o Node.o 
	cc -o edit main.o In.o Node.o 

main.o: main.c Leaves.h 
	cc -c main.c 
In.o: In.c Leaves.h
	cc -c In.c
Node.o: Node.c 
	cc -c Node.c

clean: 
	rm edit main.o In.o Node.o
