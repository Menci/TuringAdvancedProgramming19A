main: splay.so splay.h
	cc main.c ./libsplay.so -o main
splay.so: splay.c
	cc splay.c -fPIC -shared -o libsplay.so 
clean:
	rm -rf main
	rm -rf *.o
	rm -rf *.so
