all_obj = main.o josephus.o my_linked_list.o

josephus_ring : $(all_obj)
	gcc -O2 -o josephus_ring $(all_obj) -w
main.o : main.c
	gcc -O2 -c main.c -w
joseph.o : josephus.c josephus.h
	gcc -O2 -c josephus.c -w
my_linked_list.o : my_linked_list.c my_linked_list.h
	gcc -O2 -c my_linked_list.c -w

clean : 
	rm josephus_ring $(all_obj)