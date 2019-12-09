ALL = intc main

main : main.o spg.so
	gcc -o main main.o ./spg.so

intc : intc_malloc.o intc_main.o intc_spg.o
	gcc -I. -o intc intc_main.o intc_spg.o intc_malloc.o
intc_malloc.o : my_malloc.c
	gcc -o intc_malloc.o -DCOMPILETIME -c my_malloc.c
intc_main.o : main.c Scapegoat_tree.h
	gcc -I. -o intc_main.o -c main.c
intc_spg.o : Scapegoat_tree.h Scapegoat_tree.c
	gcc -I. -o intc_spg.o -c Scapegoat_tree.c

main.o : main.c Scapegoat_tree.h
	gcc -o main.o -c main.c
spg.so : Scapegoat_tree.h Scapegoat_tree.c
	gcc -o spg.so -shared -fpic Scapegoat_tree.c -ldl

clean : 
	rm -rf *.o *.so $(ALL)
