objects = Josephus main.o list.o

Josephus: main.o list.o
	gcc main.o list.o -o Josephus

main.o: main.c list.h
	gcc -c main.c
list.o: list.c list.h
	gcc -c list.c

.PHONY: clean
clean:
	rm -f $(objects)