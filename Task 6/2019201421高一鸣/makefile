JosephusProblem: mem.o data.o linkedlist.o main.o
	gcc mem.o data.o linkedlist.o main.o -o JosephusProblem
main.o: data.h linkedlist.h main.c
	gcc -c main.c
mem.o: mem.h mem.c
	gcc -c mem.c 
data.o: data.h data.c
	gcc -c data.c
linkedlist.o: linkedlist.h linkedlist.c
	gcc -c linkedlist.c
clean:
	rm -f mem.o data.o linkedlist.o main.o JosephusProblem
	
