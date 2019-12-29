#include <stdio.h>
#include <stdlib.h>

#include "basic.h"
#include "hulffman.h"
#include "bracket_sequence.h"


int main(int argc, uchar *argv[])
{
	FILE *input = fopen(argv[1], "rb");

	size_t siz = calc_file_size(input);
	size_t n = siz / (sizeof(node));

	node *tree = malloc(siz);
	fread(tree, sizeof(node), n, input);
	
	FILE *output = fopen("zipped.out", "wb");

	int root = get_root(tree, n);

	get_brckt_seq(tree, root, n, output);
	//get_weight(tree, root, output);

	hulffman_encode(tree, root, n, output);
	fflush(output);
	
	return 0;
}
