#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitio.h"
#include "errhand.h"
#include "main.h"

int main(int argc, char *argv[])
{
	FILE *output;
	BIT_FILE *input;
	
	setbuf(stdout, NULL);
	if(argc < 3)
		usage_exit(argv[0]);
	input = OpenInputBitFile(argv[1]);
	if(input == NULL || input -> file == NULL)
		fatal_error("Error opening %s for input\n", argv[1]);
	output = fopen(argv[2], "wb");
	if(output == NULL)
		fatal_error("Error opening %s for output\n", argv[2]);
	printf("\nExpanding %s to %s\n", argv[1], argv[2]);
	printf("Using %s\n", CompressionName);
	argc -= 3;
	argv += 3;
	ExpandFile(input, output, argc, argv);
	CloseInputBitFile(input);
	fclose(output);
	putc('\n',stdout);
	return 0;
}
