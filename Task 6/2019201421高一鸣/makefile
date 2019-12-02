JosephusProblem: mem.o data.o linkedlist.o main.o
	gcc mem.o data.o linkedlist.o main.o -o JosephusProblem
main.o: data.h linkedlist.h
	gcc -c main.c
mem.o: mem.h
	gcc -c mem.c 
data.o: data.h
	gcc -c data.c
linkedlist.o: linkedlist.h
	gcc -c linkedlist.c
clean:
	rm -f mem.o data.o linkedlist.o main.o JosephusProblem
	
