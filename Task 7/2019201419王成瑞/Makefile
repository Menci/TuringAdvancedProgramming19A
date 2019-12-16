ALL_OBJ = tmp/main.o tmp/memcheck.o tmp/splay.o
ALL_FILE = main.c memcheck.c splay.c memcheck.h splay.h

Release: $(ALL_OBJ)
	gcc $(ALL_OBJ) -o main -O2 -std=c11

Detail: $(ALL_FILE)
	gcc main.c memcheck.c splay.c -o main -O2 -std=c11 -DMEM_CHECK

tmp/main.o: main.c
	if [ ! -d "tmp" ]; then mkdir tmp; fi;
	gcc main.c -c -o tmp/main.o

tmp/memcheck.o: memcheck.h memcheck.c
	if [ ! -d "tmp" ]; then mkdir tmp; fi;
	gcc memcheck.c -c -o tmp/memcheck.o

tmp/splay.o: splay.h splay.c
	if [ ! -d "tmp" ]; then mkdir tmp; fi;
	gcc splay.c -c -o tmp/splay.o

clean:
	rm ./main
	rm -r tmp
