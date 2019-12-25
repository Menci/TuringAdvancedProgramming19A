Josephus : structure.o jostar.o main.o
	cc -o Josephus structure.o jostar.o main.o

structure.o : structure.c everything.h
	cc -c structure.c
jostar.o : jostar.c everything.h
	cc -c jostar.c
main.o : main.c everything.h
	cc -c main.c

clean :
	rm Josephus jostar.o main.o structure.o
