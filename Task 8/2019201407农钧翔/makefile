zip : extract.o zip.o main.o
	cc -o zip extract.o zip.o main.o -g

extract.o : extract.c everything.h
	cc -c extract.c -g
zip.o : zip.c everything.h
	cc -c zip.c -g
main.o : main.c everything.h
	cc -c main.c -g

clean :
	rm zip extract.o zip.o main.o