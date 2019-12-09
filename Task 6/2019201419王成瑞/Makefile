Release:
	make list > /dev/null
	make parseargs > /dev/null
	make main > /dev/null
	gcc tmp/*.o -o Josephus

list: list.c list.h
	if [ ! -d "tmp" ]; then mkdir tmp; fi;
	gcc -c list.c -o tmp/list.o
parseargs: parseargs.c parseargs.h
	if [ ! -d "tmp" ]; then mkdir tmp; fi;
	gcc -c parseargs.c -o tmp/parseargs.o
main: main.c
	if [ ! -d "tmp" ]; then mkdir tmp; fi;
	gcc -c main.c -o tmp/main.o

clean:
	rm Josephus
	rm -r tmp
