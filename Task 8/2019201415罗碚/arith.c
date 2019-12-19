#include <stdio.h>
#include <stdlib.h>
#include "errhand.h"
#include "bitio.h"

typedef struct
{
	unsigned short int low_count;
	unsigned short int high_count;
	unsigned short int scale;
} SYMBOL;

void build_model(FILE *input, FILE *output);
void scale_counts(unsigned long counts[], unsigned char scaled_counts[]);
void build_totals(unsigned char scaled_counts[]);
void count_bytes(FILE *input, unsigned long counts[]);
void output_counts(FILE *output, unsigned char scaled_counts[]);
void input_counts(FILE *stream);
void convert_int_to_symbol(int symbol, SYMBOL *s);
void get_symbol_scale(SYMBOL *s);
int convert_symbol_to_int(int count, SYMBOL *s);
void initialize_arithmetic_encoder();
void encode_symbol(BIT_FILE *stream, SYMBOL *s);
void flush_arithmetic_encoder(BIT_FILE *stream);
short int get_current_count(SYMBOL *s);
void initialize_arithmetic_decoder(BIT_FILE *stream);
void remove_symbol_from_stream(BIT_FILE *stream, SYMBOL *s);

#define END_OF_STREAM 256

short int totals[258];

char *CompressionName = "Fixed order 0 model witl arithmetic coding";
char *Usage = "in-file out-file\n\n";

void CompressFile(FILE *input, BIT_FILE *output, int argc, char* argv[])
{
	int c;
	SYMBOL s;
	
	build_model(input, output -> file);
	initialize_arithmetic_encoder();
	
	while((c = getc(input)) != EOF)
	{
		convert_int_to_symbol(c, &s);
		encode_symbol(output, &s);
	}
	convert_int_to_symbol(END_OF_STREAM, &s);
	encode_symbol(output, &s);
	flush_arithmetic_encoder(output);
	OutputBits(output, 0L, 16);
	while(argc-- > 0)
		printf("Unknown arguments: %s\n", *argv++);
}

void ExpandFile(BIT_FILE *input, FILE *output, int argc, char* argv[])
{
	SYMBOL s;
	int c;
	int count;
	
	input_counts(input -> file);
	initialize_arithmetic_decoder(input);
	for( ; ; )
	{
		get_symbol_scale(&s);
		count = get_current_count(&s);
		c = convert_symbol_to_int(count, &s);
		if(c == END_OF_STREAM)
			break;
		remove_symbol_from_stream(input, &s);
		putc((char)c, output);	
	}
	while(argc-- > 0)
		printf("Unknown arguments: %s\n", *argv++);
}

void build_model(FILE *input, FILE* output)
{
	unsigned long counts[256];
	unsigned char scaled_counts[256];
	
	count_bytes(input, counts);
	scale_counts(counts, scaled_counts);
	output_counts(output, scaled_counts);
	build_totals(scaled_counts);
}

#define SEEK_SET 0

void count_bytes(FILE *input, unsigned long counts[])
{
	long input_marker;
	int i, c;
	
	for(i = 0; i < 256; i++)
		counts[i] = 0;
	input_marker = ftell(input);
	while((c = getc(input)) != EOF)
		counts[c]++;
	fseek(input, input_marker, SEEK_SET);
}

void scale_counts(unsigned long counts[], unsigned char scaled_counts[])
{
	int i;
	unsigned long max_count, scale;
	unsigned int total;
	
	max_count = 0;
	for(i = 0; i < 256; i++)
		if(counts[i] > max_count)
			max_count = counts[i];
	scale = max_count / 256;
	scale = scale + 1;
	for(i = 0; i < 256; i++)
	{
		scaled_counts[i] = (unsigned char) (counts[i] / scale);
		if(scaled_counts[i] == 0 && counts[i] != 0)
			scaled_counts[i] = 1;
	}
	total = 1;
	for(i = 0; i < 256; i++)
		total += scaled_counts[i];
	if(total > (32767 - 256))
		scale = 4;
	else if(total > 16383)
		scale = 2;
	else
		return ;
	for(i = 0; i < 256; i++)
		scaled_counts[i] /= scale;
}

void build_totals(unsigned char scaled_counts[])
{
	int i;
	
	totals[0] = 0;
	for(i = 0; i < END_OF_STREAM; i++)
		totals[i + 1] = totals[i] + scaled_counts[i];
	totals[END_OF_STREAM + 1] = totals[END_OF_STREAM] + 1;
}

void output_counts(FILE *output, unsigned char scaled_counts[])
{
	int first, last, next, i;
	
	first = 0;
	while(first < 255 && scaled_counts[first] == 0)
		first++;
	for( ; first < 256; first = next)
	{
		last = first + 1;
		for( ; ; )
		{
			for( ; last < 256; last++)
				if(scaled_counts[last] == 0)
					break;
			last--;
			for(next = last + 1; next < 256; next++)
				if(scaled_counts[next] != 0)
					break;
			if(next > 255)
				break;
			if((next - last) > 3)
				break;
			last = next; 
		}
		
		if(putc(first, output) != first)
			fatal_error("Error writing bytes counts\n");
		if(putc(last, output) != last)
			fatal_error("Error writing bytes counts\n");
		for(i = first; i <= last; i++)
			if(putc(scaled_counts[i], output) != (int) scaled_counts[i])
				fatal_error("Error writing bytes counts\n");
	}
	if(putc(0, output) != 0)
		fatal_error("Error writing bytes counts\n");
}

void input_counts(FILE *input)
{
	int first, last, i, c;
	unsigned char scaled_counts[256];
	
	for(i = 0; i < 256; i++)
		scaled_counts[i] = 0;
	if((first = getc(input)) == EOF)
		fatal_error("Error reading byte counts\n");
	if((last = getc(input)) == EOF)
		fatal_error("Error reading byte counts\n");
	for( ; ; )
	{
		for(i = first; i <= last; i++)
			if((c = getc(input)) == EOF)
				fatal_error("Error reading byte counts\n");
			else
				scaled_counts[i] = (unsigned char) c;
		if((first = getc(input)) == EOF)
			fatal_error("Error reading byte counts\n");
		if(first == 0)
			break;
		if((last = getc(input)) == EOF)
			fatal_error("Error reading byte counts\n");
	}
	build_totals(scaled_counts);
}

static unsigned short int code;
static unsigned short int low;
static unsigned short int high;
long underflow_bits;

void initialize_arithmetic_encoder()
{
	low = 0;
	high = 0xffff;
	underflow_bits = 0;
}

void flush_arithmetic_encoder(BIT_FILE *stream)
{
	OutputBit(stream, low & 0x4000);
	underflow_bits++;
	while(underflow_bits-- > 0)
		OutputBit(stream, ~low & 0x4000);
}

void convert_int_to_symbol(int c, SYMBOL *s)
{
	s -> scale = totals[END_OF_STREAM + 1];
	s -> low_count = totals[c];
	s -> high_count = totals[c + 1];
}

void get_symbol_scale(SYMBOL *s)
{
	s -> scale = totals[END_OF_STREAM + 1];
}

int convert_symbol_to_int(int count, SYMBOL *s)
{
	int c;
	
	for(c = END_OF_STREAM; count < totals[c]; c--);
	
	s -> high_count = totals[c + 1];
	s -> low_count = totals[c];
	return (c);
}

void encode_symbol(BIT_FILE *stream, SYMBOL *s)
{
	long range;
	
	range = (long)(high - low) + 1;
	high = low + (unsigned short int) ((range * s -> high_count) / s -> scale - 1);
	low = low + (unsigned short int) ((range * s -> low_count) / s -> scale);
	
	for( ; ; )
	{
		if((high & 0x8000) == (low & 0x8000))
		{
			OutputBit(stream, high & 0x8000);
			while(underflow_bits > 0)
			{
				OutputBit(stream, ~high & 0x8000);
				underflow_bits--;
			}
		}
		else if((low & 0x4000) && !(high & 0x4000))
		{
			underflow_bits += 1;
			low &= 0x3fff;
			high |= 0x4000;
		}
		else
			return ;
		low <<= 1;
		high <<= 1;
		high |= 1;
	}
}

short int get_current_count(SYMBOL *s)
{
	long range;
	short int count;
	
	range = (long)(high - low) + 1;
	count = (short int) ((((long)(code - low) + 1) * s -> scale - 1) / range);
	return (count);
}

void initialize_arithmetic_decoder(BIT_FILE* stream)
{
	int i;
	
	code = 0;
	for(i = 0; i < 16; i++)
	{
		code <<= 1;
		code += InputBit(stream);
	}
	low = 0;
	high = 0xffff;
}

void remove_symbol_from_stream(BIT_FILE *stream, SYMBOL *s)
{
	long range;
	
	range = (long)(high - low) + 1;
	high = low + (unsigned short int) ((range * s -> high_count) / s -> scale - 1);
	low = low + (unsigned short int) ((range * s -> low_count) / s -> scale);
	
	for( ; ; )
	{
		if((high & 0x8000) == (low & 0x8000))
		{
		}
		else if((low & 0x4000) == 0x4000 && (high & 0x4000) == 0)
		{
			code ^= 0x4000;
			low &= 0x3fff;
			high |= 0x4000;
		}
		else
			return;
		low <<= 1;
		high <<= 1;
		high |= 1;
		code <<= 1;
		code += InputBit(stream);
	}
}
