Solve:Solve.o RB_Tree.so
	gcc  -o Solve Solve.o ./RB_Tree.so
Solve.o:Solve.c RB_Tree.h
	gcc  -c Solve.c RB_Tree.h
RB_Tree.so:RB_Tree.c RB_Tree.h
	gcc -shared -fpic -o RB_Tree.so RB_Tree.c  RB_Tree.h
clean:
	rm *.o *.so Solve ans.out
