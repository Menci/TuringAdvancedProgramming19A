temp = main.o zip_tree.o unzip_tree.o judge.o lzw.o treap.o
exe = zip
K = -D_K=8

zip:$(temp) makefile
	gcc -g -o $(exe) $(temp)

main.o:main.c main.h struct.h
	gcc -c main.c -g $(K)

zip_tree.o:zip_tree.c zip_tree.h struct.h
	gcc -c zip_tree.c -g $(K)

unzip_tree.o:unzip_tree.c	unzip_tree.h struct.h
	gcc -c unzip_tree.c -g $(K)

judge.o:judge.c judge.h struct.h
	gcc -c judge.c -g $(K)

lzw.o:lzw.c lzw.h struct.h
	gcc -c lzw.c -g $(K)

treap.o:treap.c treap.h struct.h
	gcc -c treap.c -g $(K)

clean:
	rm $(temp) $(exe)

run: zip
	./zip

debug: 
	gdb $(exe)