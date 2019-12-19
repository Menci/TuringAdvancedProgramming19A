#include <stdio.h>

typedef struct bit_file
{
	FILE* file;
	unsigned char mask;
	int rack;
	int pacifier_counter;
} BIT_FILE;

BIT_FILE *OpenInputBitFile(char *name);
BIT_FILE *OpenOutputBitFile(char *name);
void OutputBit(BIT_FILE *bit_file, int bit);
void OutputBits(BIT_FILE *bit_file, unsigned long code, int count);
int InputBit(BIT_FILE *bit_file);
unsigned long InputBits(BIT_FILE *bit_file, int bit_count);
void CloseInputBitFile(BIT_FILE *bit_file);
void CloseOutputBitFile(BIT_FILE *bit_file);
void FilePrintBinary(FILE *file, unsigned int code, int bits);
