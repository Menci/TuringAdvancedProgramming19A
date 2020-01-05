#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "errhand.h"
#include "bitio.h"

#define BITS 15
#define MAX_CODE ((1<<BITS)-1)
#define TABLE_SIZE 35023L
#define TABLE_BANKS ((TABLE_SIZE>>8)+1)
#define END_OF_STREAM 256
#define BUMP_CODE 257
#define FLUSH_CODE 258
#define FIRST_CODE 259
#define UNUSED -1

unsigned int find_child_node(int parent_code, int child_character);
unsigned int decode_string(unsigned int offset, unsigned int code);

char *CompressionName = "LZW 15 BIT Variable Rate Encoder";
char *Usage           = "in-file out-file\n\n";

struct dictionary
{
	int code_value;
	int parent_code;
	char character;
} * dict[TABLE_BANKS];

#define DICT(i) dict[(i)>>8][(i)&0xff]

char decode_stack[TABLE_SIZE];
unsigned int next_code;
int current_code_bits;
unsigned int next_bump_code;

void InitializeDictionary()
{
	unsigned int i;

	for(i = 0; i < TABLE_SIZE; i++)
		DICT(i).code_value = UNUSED;
	next_code = FIRST_CODE;
//	putc('F',stdout);
	current_code_bits = 9;
	next_bump_code = 511; 
}

void InitializeStorage()
{
	int i;

	for(i = 0; i < TABLE_BANKS; i++)
	{
		dict[i] = (struct dictionary *) calloc(256, sizeof(struct dictionary));
		if(dict[i] == NULL)
			fatal_error("Error allocating dictionary space");
	}
}

void CompressFile(FILE* input, BIT_FILE* output, int argc, char* argv[])
{
	int character;
	int string_code;
	unsigned int index;

	InitializeStorage();
	InitializeDictionary();
	if((string_code = getc(input)) == EOF)
		string_code = END_OF_STREAM;
	while((character = getc(input)) != EOF)
	{
		index = find_child_node(string_code, character);
		if(DICT(index).code_value != -1)
			string_code = DICT(index).code_value;
		else
		{
			DICT(index).code_value = next_code++;
			DICT(index).parent_code = string_code;
			DICT(index).character = (char) character;
			OutputBits(output, (unsigned long) string_code, current_code_bits);
			string_code = character;
			if(next_code > MAX_CODE)
			{
				OutputBits(output, (unsigned long) FLUSH_CODE, current_code_bits);
				InitializeDictionary();
			}
			else if(next_code > next_bump_code)
			{
				OutputBits(output, (unsigned long) BUMP_CODE, current_code_bits);
				current_code_bits++;
				next_bump_code <<= 1;
				next_bump_code |= 1;
			//	putc('B',stdout);
			}
		}
	}
	OutputBits(output, (unsigned long) string_code, current_code_bits);
	OutputBits(output, (unsigned long) END_OF_STREAM, current_code_bits);
	while(argc-- > 0)
		printf("Unknown argument %s\n", *argv++);
}

void ExpandFile(BIT_FILE* input, FILE* output, int argc, char* argv[])
{
	unsigned int new_code;
	unsigned int old_code;
	int character;
	unsigned int count;

	InitializeStorage();
	while(argc-- > 0)
		printf("Unknown argument: %s\n", *argv++);
	for( ; ; )
	{
		InitializeDictionary();
		old_code = (unsigned int) InputBits(input, current_code_bits);
		if(old_code == END_OF_STREAM)
			return ;
		character = old_code;
		putc(old_code, output);
		for( ; ; )
		{
			new_code = (unsigned int) InputBits(input, current_code_bits);
			if(new_code == END_OF_STREAM)
				return ;
			if(new_code == FLUSH_CODE)
				break;
			if(new_code == BUMP_CODE)
			{
				current_code_bits++;
			//	putc('B',stdout);
				continue;
			}
			if(new_code >= next_code)
			{
				decode_stack[0] = (char) character;
				count = decode_string(1, old_code);
			}
			else
				count = decode_string(0, new_code);
			character = decode_stack[count - 1];
			while(count > 0)
				putc(decode_stack[--count], output);
			DICT(next_code).parent_code = old_code;
			DICT(next_code).character = (char) character;
			next_code++;
			old_code = new_code;
		}
	}
}

unsigned int find_child_node(int parent_node, int child_character)
{
	unsigned int index;
	int offset;

	index = (child_character << (BITS - 8)) ^ parent_node;
	if(index == 0)
		offset = 1;
	else
		offset = TABLE_SIZE - index;
	for( ; ; )
	{
		if(DICT(index).code_value == UNUSED)
			return ((unsigned int) index);
		if(DICT(index).parent_code == parent_node && DICT(index).character == (char) child_character)
			return (index);
		if((int) index >= offset)
			index -= offset;
		else
			index += TABLE_SIZE - offset;
	}
}

unsigned int decode_string(unsigned int count, unsigned int code)
{
	while(code > 255)
	{
		decode_stack[count++] = DICT(code).character;
		code = DICT(code).parent_code;
	}
	decode_stack[count++] = (char)code;
	return (count);
}
