all_obj = main.o tree.o decode.o encode.o fileio.o

converter : $(all_obj) tree.h
	gcc -O2 -o converter $(all_obj) -DK=${K} -w
main.o : main.c tree.h
	gcc -O2 -c main.c -DK=${K} -w
tree.o : tree.h tree.c
	gcc -O2 -c tree.c -DK=${K} -w
decode.o : decode.h decode.c tree.h
	gcc -O2 -c decode.c -DK=${K} -w
encode.o : encode.h encode.c tree.h
	gcc -O2 -c encode.c -DK=${K} -w
fileio.o : fileio.h fileio.c tree.h
	gcc -O2 -c fileio.c -DK=${K} -w

clean : 
	-rm $(all_obj) converter