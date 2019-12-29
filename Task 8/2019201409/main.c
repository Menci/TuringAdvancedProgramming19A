#include "tree.h"
#include "fileio.h"
#include "encode.h"
#include "decode.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("Invalid Argument.\n");
		return 1;
	}
	int oper = atoi(argv[1]);
	if(oper == 0) { // Encode
		size_t n, outsize;
		node *in = readOriginal(&n, argv[2]);
		void *out = encodeStructToArray(in, &outsize, n);
		printf("[Compress] zipped.bin = %d Bytes\n", outsize);
		writeCoded(out, outsize);
		free(in);
		free(out);
		return 0;
	}
	else if(oper == 1) { // Decode & Check
		size_t n, insize;
		void *in = readCoded(&insize);
		node *std = readOriginal(&n, argv[2]);
		node *my = decodeArrayToStruct(in, insize / (K + 1));
		free(in);
		checkStructure(std, my);
		free(my);
		printf("[Decompress] Check Passed!\n");
		return 0;
	}
	else {
		printf("Invalid Argument.\n");
		return 1;
	}
}