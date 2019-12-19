#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errhand.h"
#include "bitio.h"

char *CompressionName = "Tree Struct Compression Encoder";
char *Usage = "in-file out-file\n";

struct Node {
    int left_child, right_child;
    char data[K];
} nodes[N + 3]; 

unsigned short int kh[4 * N + 4];
int skh;
void OutputTree(int, BIT_FILE *);
int siz;
int ExpandTree(BIT_FILE *);

void CompressFile(FILE *input, BIT_FILE *output, int argc, char *argv[])
{
	int i;
	if(fread(nodes, sizeof(struct Node), N, input) < N)
		fatal_error("Error reading N nodes from in-file, maybe N in *.cc is not correct\n");
	skh=0;
	siz=0;
	OutputTree(0, output);
	for(i = 0; i < skh; i++)
		OutputBit(output, kh[i]);
	
	while(argc-- > 0)
		printf("Unknown argument %s\n", *argv++);
}

void ExpandFile(BIT_FILE *input, FILE *output, int argc, char *argv[])
{
	int i, j;
	for(i = 0; i < N; i++)
		for(j = 0; j < K; j++)
			nodes[i].data[j] = InputBits(input, 8);
	siz = 0;
	if(ExpandTree(input) == -1)
		fatal_error("Tree Expanding Error\n");
	
	if(fwrite(nodes, sizeof(struct Node), N, output)!=N)
		fatal_error("Error Writing Data into out-file\n");
	
	while(argc-- > 0)
		printf("Unknown argument %s\n", *argv++);
}

void OutputTree(int x, BIT_FILE *output)
{
	int i;
	kh[skh++] = 0;
	if(x != -1) 
	{
		siz++;
		for(i = 0; i < K; i++)
			OutputBits(output, nodes[x].data[i], 8);
		OutputTree(nodes[x].left_child, output);
		OutputTree(nodes[x].right_child, output);
	}
	kh[skh++] = 1;
}

int ExpandTree(BIT_FILE *input)
{
	if(InputBit(input))
		return -2;
	
	int ord = siz++;
	if((nodes[ord].left_child = ExpandTree(input)) == -2)
	{
		siz--;
		return -1;
	}
	nodes[ord].right_child = ExpandTree(input);
	
	if(InputBit(input) == 0)
		fatal_error("Error Tree Expanding\n");
	
	return ord;
}
