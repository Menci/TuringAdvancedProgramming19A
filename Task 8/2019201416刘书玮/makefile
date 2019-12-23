make: main.c bracket.o input.o output.o huffman.o compress.o tool.o
	gcc main.c bracket.o input.o output.o huffman.o compress.o tool.o -g -o main
bracket.o: bracket.c tool.o
	gcc -c bracket.c -o bracket.o -g
input.o: input.c
	gcc -c input.c -o input.o -g
output.o: output.c
	gcc -c output.c -o output.o -g
huffman.o: huffman.c
	gcc -c huffman.c -o huffman.o -g
compress.o: compress.c
	gcc -c compress.c -o compress.o -g
	
check: decompress.o deinput.o deoutput.o dehuffman.o debracket.o tool.o
	gcc check.c decompress.o deinput.o deoutput.o dehuffman.o debracket.o tool.o -o check -g
decompress.o:
	gcc -c decompress.c -o decompress.o -g
deinput.o:
	gcc -c deinput.c -o deinput.o -g
deoutput.o:
	gcc -c deoutput.c -o deoutput.o -g
dehuffman.o:
	gcc -c dehuffman.c -o dehuffman.o -g
debracket.o:
	gcc -c debracket.c -o debracket.o -g

tool.o: tool.c
	gcc -c tool.c -o tool.o -g
clean:
	rm *.o
	rm main
	rm clean
