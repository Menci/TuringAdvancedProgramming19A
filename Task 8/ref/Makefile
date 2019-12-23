all: check main.c bitset/bitset.c bitset/bitset.h bitset/bitstream.c bitset/bitstream.h huffman/huffman.c huffman/huffman.h int256/int256.c int256/int256.h serialization/serialize.c serialization/serialize.h
	cc -DN=$(N) -DK=$(K) main.c bitset/bitset.c bitset/bitstream.c huffman/huffman.c int256/int256.c serialization/serialize.c -o ref -O2
clean:
	rm -f ref

.PHONY: check
check:
ifndef N
	$(error N is undefined)
endif
ifndef K
	$(error K is undefined)
endif
