main : main.c ptreap
	gcc -O2 main.c -o main -L. -lptreap -Wl,-rpath=. -Wno-unused-result
ptreap : persistent_treap.o persistent_treap_node.o
	gcc -O2 -shared -fPIC -o libptreap.so persistent_treap.o persistent_treap_node.o
persistent_treap.o : persistent_treap.c persistent_treap.h persistent_treap_node.o
	gcc -O2 -fPIC -c persistent_treap.c
persistent_treap_node.o : persistent_treap_node.c persistent_treap_node.h
	gcc -O2 -fPIC -c persistent_treap_node.c
malloc_debug.so : malloc_debug.c
	gcc -DRUNTIME -D_GNU_SOURCE -shared -fPIC -o malloc_debug.so malloc_debug.c -ldl

clean :
	rm persistent_treap_node.o persistent_treap.o main libptreap.so malloc_debug.so