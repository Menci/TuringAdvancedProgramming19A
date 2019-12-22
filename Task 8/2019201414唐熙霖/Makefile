K := 64

All: main checker

main: basic.o hulffman.o brckt.o main.o
	gcc basic.o hulffman.o brckt.o main.o -DK=$(K) -o main -g

basic.o: basic.h basic.c
	gcc -c basic.c -DK=$(K)8 -o basic.o
brckt.o: basic.h  bracket_sequence.h bracket_sequence.c
	gcc -c bracket_sequence.c -DK=$(K) -o brckt.o
hulffman.o: basic.h hulffman.h hulffman.c
	gcc -c hulffman.c -DK=$(K) -o hulffman.o
main.o: basic.h bracket_sequence.h hulffman.h main.c
	gcc -c main.c -DK=$(K) -o main.o

checker: basic.o hulffman.o brckt.o checker.o
	gcc basic.o hulffman.o brckt.o checker.o -DK=$(K) -o checker -g
checker.o: basic.h bracket_sequence.h hulffman.h checker.c
	gcc -c checker.c -DK=$(K) -o checker.o

clean:
	rm -rf *.o
	rm -rf ./main
	rm -rf ./checker
	rm -rf zipped.out

