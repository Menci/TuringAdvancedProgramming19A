#include "tree.h"
#include "fileio.h"
#include "encode.h"
#include "decode.h"
#include <stdio.h>
int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Invalid Argument.\n");
		return 1;
	}
	int oper = atoi(argv[1]);
	if(oper == 0) { // Encode
		int n, outsize;
		node *in = (node *)readOriginal(&n);
		void *out = encodeStructToArray(in, &outsize, n);
		writeCompressed(out, outsize);
		free(in);
		free(out);
		return 0;
	}
	else if(oper == 1) { // Decode & Check
		int n, insize;
		void *in = readCompressed(&insize);
		node *my = decodeArrayToStruct(in, insize);
		free(in);
		free(my);
		return 0;
	}
	else {
		printf("Invalid Argument.\n");
		return 1;
	}
}