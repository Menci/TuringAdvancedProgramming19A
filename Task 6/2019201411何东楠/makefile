Joseph : main.o joseph_worker.o Dlinked_list.o
	gcc -o Joseph main.o joseph_worker.o Dlinked_list.o
main.o : main.c joseph_worker.c Dlinked_list.c
	gcc -c main.c
joseph_worker.o : joseph_worker.c Dlinked_list.c
	gcc -c joseph_worker.c
Dlinked_list.o : Dlinked_list.c
	gcc -c Dlinked_list.c
clean :
	rm Joseph *.o
