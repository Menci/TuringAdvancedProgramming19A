BST:libbinary_search_tree.so main.o mymalloc.so
	gcc -o BST main.o -lbinary_search_tree
main.o:main.c binary_search_tree.h
	gcc -c main.c
libbinary_search_tree.so:binary_search_tree.c binary_search_tree.h
	gcc binary_search_tree.c -fPIC -shared -o libbinary_search_tree.so
	mv libbinary_search_tree.so /usr/lib
mymalloc.so:mymalloc.c
	gcc -DRUNTIME -shared -fpic -o mymalloc.so mymalloc.c -ldl
clean:
	rm -r BST main.o /usr/lib/libbinary_search_tree.so mymalloc.so
