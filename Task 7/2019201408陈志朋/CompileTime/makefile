Solve:Solve.o RB_Tree.o mymalloc.o RB_Tree.h malloc.h
	gcc  -o Solve Solve.o RB_Tree.o mymalloc.o
Solve.o:Solve.c RB_Tree.h
	gcc  -c Solve.c
RB_Tree.o:RB_Tree.c RB_Tree.h malloc.h
	gcc -I. -c RB_Tree.c
mymalloc.o:mymalloc.c
	gcc -DCOMPILETIME -c mymalloc.c
clean:
	rm *.o Solve
