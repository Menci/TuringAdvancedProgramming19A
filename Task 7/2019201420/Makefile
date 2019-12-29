a: splay_tree.o a.o
	gcc -o a splay_tree.o a.o
a.o: splay_tree.h
	gcc -c a.c
splay_tree.o: splay_tree.h splay_tree.c
	gcc -c splay_tree.c
.PHONY: clear
clear: 
	rm -f a splay_tree.o a.o
