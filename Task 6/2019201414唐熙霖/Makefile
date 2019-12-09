Josephus : solve.o main.o
	gcc -o Josephus solve.o main.o

solve.o : linked_list.h solve.c
	gcc -c solve.c
main.o : solve.c
	gcc -c main.c

clean :
	rm -rf *.o
	rm -rf Josephus

