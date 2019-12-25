main: main.o work.o josephus.h
	gcc -o main main.o work.o
work.o: work.c josephus.h
	gcc -c work.c
main.o: main.c josephus.h
	gcc -c main.c
clean:
	rm main main.o work.o
