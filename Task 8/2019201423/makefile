treezip: main.o zip.o tree.o
	gcc main.o zip.o tree.o -o treezip -O2 -std=c11 -g
main.o: main.c zip.h
	gcc -c main.c -O2 -std=c11 -g
zip.o: zip.c zip.h tree.h
	gcc -c zip.c -O2 -std=c11 -g
tree.o: tree.c tree.h
	gcc -c tree.c -O2 -std=c11 -g
std: std.o tree.o
	gcc std.o tree.o -o std -O2 -std=c11 -g
std.o: std.c tree.h
	gcc -c std.c -O2 -std=c11 -g

.PHONY: clean
clean:
	rm -f treezip std main.o zip.o tree.o std.o *.bin