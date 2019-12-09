temp = libtreap.so
exe = treap

$(exe):$(temp) main.h
	gcc main.c -L. -ltreap -o $(exe)
	
libtreap.so:treap.c treap.h struct.h malloc.c
	gcc -O2 -fPIC -shared treap.c -o libtreap.so

run:
	LD_LIBRARY_PATH=. ./$(exe)

clean:
	rm $(temp) $(exe)