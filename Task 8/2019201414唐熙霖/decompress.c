#include <stdio.h>
#include <stdlib.h>

#include "basic.h"
#include "hulffman.h"
#include "bracket_sequence.h"

int main()
{
	FILE *compressed = fopen("zipped.out", "rb");
	FILE *output = fopen("decompressed.out", "wb");
	
	int brckt_num;
	fread(&brckt_num, sizeof(int), 1, compressed);
	uchar *seq = (char *)malloc(sizeof(uchar)*((brckt_num>>3) + 1));
	fread(seq, sizeof(uchar), (brckt_num>>3) + 1, compressed);

	decode_brckt_seq(brckt_num, seq, output);

	int code_num;
	fread(&code_num, sizeof(int), 1, compressed);
	code_table *table = (code_table *)malloc(sizeof(code_table) * code_num);
	fread(table, sizeof(code_table), code_num, compressed);
//exit(0);
	hulffman_decode(code_num, table, compressed, output);
	
	return 0;
}
