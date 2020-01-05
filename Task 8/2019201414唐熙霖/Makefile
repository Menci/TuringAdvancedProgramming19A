#file_name := 1.in
#K := 4
#file_name := 2.in
#K := 8
#file_name := 3.in
#K := 64
#file_name := 4.in
#K := 8
#file_name := 5.in
#K := 8
#file_name := 6.in
#K := 8
#file_name := 7.in
#K := 8
#file_name := 8.in
#K := 8

All: main decompress checker run

main: basic.o hulffman.o brckt.o main.o
	gcc basic.o hulffman.o brckt.o main.o -DK=$(K) -o main -g
main.o: basic.h bracket_sequence.h hulffman.h main.c
	gcc -c main.c -DK=$(K) -o main.o

basic.o: basic.h basic.c
	gcc -c basic.c -DK=$(K) -o basic.o
brckt.o: basic.h  bracket_sequence.h bracket_sequence.c
	gcc -c bracket_sequence.c -DK=$(K) -o brckt.o
hulffman.o: basic.h hulffman.h hulffman.c
	gcc -c hulffman.c -DK=$(K) -o hulffman.o

checker: basic.o hulffman.o brckt.o checker.o
	gcc basic.o hulffman.o brckt.o checker.o -DK=$(K) -o checker -g
checker.o: basic.h bracket_sequence.h hulffman.h checker.c
	gcc -c checker.c -DK=$(K) -o checker.o

decompress: basic.o hulffman.o brckt.o decompress.o
	gcc basic.o hulffman.o brckt.o decompress.o -DK=$(K) -o decompress -g
decompress.o: basic.h bracket_sequence.h hulffman.h decompress.c
	gcc -c decompress.c -DK=$(K) -o decompress.o

run: main decompress main
	./main $(file_name)
	./decompress $(file_name)
	./checker $(file_name)

clean:
	rm -rf *.o
	rm -rf ./main
	rm -rf ./checker
	rm -rf ./decompress
	rm -rf zipped.out
	rm -rf decompressed.out

