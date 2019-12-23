#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errhand.h"
#include "bitio.h"

typedef struct
{
	unsigned short int low_count;
	unsigned short int high_count;
	unsigned short int scale;
} SYMBOL;

#define MAXIMUM_SCALE 16383
#define ESCAPE 256
#define DONE (-1)
#define FLUSH (-2)

void initialize_options(int argc, char *argv[]);
int check_compression(FILE *input, BIT_FILE *output);
void initialize_model();
void update_model(int symbol);
int convert_int_to_symbol(int symbol, SYMBOL *s);
void get_symbol_scale(SYMBOL *s);
int convert_symbol_to_int(int count, SYMBOL *s);
void add_character_to_model(int c);
void flush_model();
void initialize_arithmetic_decoder(BIT_FILE *stream);
void remove_symbol_from_stream(BIT_FILE *stream, SYMBOL *s);
void initialize_arithmetic_encoder();
void encode_symbol(BIT_FILE *stream, SYMBOL *s);
void flush_arithmetic_encoder(BIT_FILE *stream);
short int get_current_count(SYMBOL *s);

char *CompressionName = "Fixed order n model with arithmetic coding";
char *Usage = "in-file out-file [-o order]\n\n";
int max_order = 3;

void CompressFile(FILE *input, BIT_FILE *output, int argc, char* argv[])
{
	SYMBOL s;
	int c, escaped, flush = 0;
	long int text_count = 0;
	
	initialize_options(argc, argv);
	initialize_model();
	initialize_arithmetic_encoder();
	for( ; ; )
	{
		if((++text_count & 0x0ff) == 0)
			flush = check_compression(input, output);
		if(!flush)
			c = getc(input);
		else
			c = FLUSH;
		if(c == EOF)
			c = DONE;
		do
		{
			escaped = convert_int_to_symbol(c, &s);
			encode_symbol(output, &s);
		}
		while(escaped);
		if(c == DONE)
			break;
		if(c == FLUSH)
		{
			flush_model();
			flush = 0;
		}
		update_model(c);
		add_character_to_model(c);
	}
	flush_arithmetic_encoder(output);
}

void ExpandFile(BIT_FILE *input, FILE *output, int argc, char* argv[])
{
	SYMBOL s;
	int c, count;
	
	initialize_options(argc, argv);
	initialize_model();
	initialize_arithmetic_decoder(input);
	for( ; ; )
	{
		do
		{
			get_symbol_scale(&s);
			count = get_current_count(&s);
			c = convert_symbol_to_int(count, &s);
			remove_symbol_from_stream(input, &s); 
		}
		while(c == ESCAPE);
		if(c == DONE)
			break;
		if(c != FLUSH)
			putc((char)c, output);
		else
			flush_model();
		update_model(c);
		add_character_to_model(c); 
	}
}

void initialize_options(int argc, char* argv[])
{
	while(argc-- > 0)
	{
		if(strcmp(*argv,"-o") == 0)
		{
			argc--;
			max_order = atoi(*++argv);
		}
		else
			printf("Unknown argument on command line: %s\n", *argv);
		argc--;
		argv++;
	}
}

int check_compression(FILE *input, BIT_FILE *output)
{
	static long local_input_marker = 0L;
	static long local_output_marker = 0L;
	long total_input_bytes;
	long total_output_bytes;
	int local_ratio;
	total_input_bytes = ftell(input) - local_input_marker;
	total_output_bytes = ftell(output -> file);
	total_output_bytes -= local_output_marker;
	if(total_output_bytes == 0)
		total_output_bytes = 1;
	local_ratio = (int) ((total_output_bytes * 100) / total_input_bytes);
	
	local_input_marker = ftell(input);
	local_output_marker = ftell(output -> file);
	
	return (local_ratio > 90);
}

typedef struct
{
	unsigned char symbol;
	unsigned char counts;
} STATS;

typedef struct
{
	struct context *next;
} LINKS;

typedef struct context
{
	int max_index;
	LINKS *links;
	STATS *stats;
	struct context *lesser_context;
} CONTEXT;

CONTEXT **contexts;

int current_order;

short int totals[258];
char scoreboard[256];

void update_table(CONTEXT *table, int symbol);
void rescale_table(CONTEXT *table);
void totalize_table(CONTEXT *table);
CONTEXT* shift_to_next_context(CONTEXT *table, int c, int order);
CONTEXT* allocate_next_order_table(CONTEXT *table, int symbol, CONTEXT * lesser_context);
void recursive_flush(CONTEXT *table);

void initialize_model()
{
	int i;
	CONTEXT *null_table;
	CONTEXT *control_table;
	
	current_order = max_order;
	contexts = (CONTEXT **)calloc(sizeof(CONTEXT *), 10);
	if(contexts == NULL)
		fatal_error("Failure #1: allocating context table!");
	contexts += 2;
	null_table = (CONTEXT *) calloc(sizeof(CONTEXT *), 1);
	if(null_table == NULL)
		fatal_error("Failure #2: allocating null table!");
	null_table -> max_index = -1;
	contexts[-1] = null_table;
	for(i = 0; i <= max_order; i++)
		contexts[i] = allocate_next_order_table(contexts[i - 1], 0, contexts[i - 1]);
	free((char *) null_table -> stats);
	null_table -> stats = (STATS *) calloc(sizeof(STATS), 256);
	if(null_table -> stats == NULL)
		fatal_error("Failure #3: allocating null table!");
	null_table -> max_index = 255;
	for(i = 0; i < 256; i++)
	{
		null_table -> stats[i].symbol = (unsigned char) i;
		null_table -> stats[i].counts = 1;
	}
	
	control_table = (CONTEXT *) calloc(sizeof(CONTEXT), 1);
	if(control_table == NULL)
		fatal_error("Failure #4: allocating null table!");
	control_table -> stats = (STATS *) calloc(sizeof(STATS), 2);
	if(control_table -> stats == NULL)
		fatal_error("Failure #5: allocating null table!");
	contexts[-2] = control_table;
	control_table -> max_index = 1;
	control_table -> stats[0].symbol = -FLUSH;
	control_table -> stats[0].counts = 1;
	control_table -> stats[1].symbol = -DONE;
	control_table -> stats[1].counts = 1;
	
	for(i = 0; i < 256; i++)
		scoreboard[i] = 0;
}

CONTEXT *allocate_next_order_table(CONTEXT *table, int symbol, CONTEXT *lesser_context)
{
	CONTEXT *new_table;
	int i;
	unsigned int new_size;
	
	for(i = 0; i <= table -> max_index; i++)
		if(table -> stats[i].symbol == (unsigned char) symbol)
			break;
	if(i > table -> max_index)
	{
		table -> max_index ++;
		new_size = sizeof(LINKS);
		new_size *= table -> max_index + 1;
		if(table -> links == NULL)
			table -> links = (LINKS *) calloc(new_size, 1);
		else
			table -> links = (LINKS *) realloc((char *)table -> links, new_size);
		new_size = sizeof(STATS);
		new_size *= table -> max_index + 1;
		if(table -> stats == NULL)
			table -> stats = (STATS *) calloc(new_size, 1);
		else
			table -> stats = (STATS *) realloc((char *)table -> stats, new_size);
		if(table -> links == NULL)
			fatal_error("Failure #6: allocating new table!");
		if(table -> stats == NULL)
			fatal_error("Failure #7: allocating new table!");
		table -> stats[i].symbol = (unsigned char) symbol;
		table -> stats[i].counts = 0;
	}
	new_table = (CONTEXT *) calloc(sizeof(CONTEXT), 1);
	if(new_table == NULL)
		fatal_error("Failure #8: allocating new table!");
	new_table -> max_index = -1;
	table -> links[i].next = new_table;
	new_table -> lesser_context = lesser_context;
	return (new_table);
}

void update_model(int symbol)
{
	int i, local_order;
	
	if(current_order < 0)
		local_order = 0;
	else
		local_order = current_order;
	if(symbol >= 0)
	{
		while(local_order <= max_order)
		{
			if(symbol >= 0)
				update_table(contexts[local_order], symbol);
			local_order++;
		}
	}
	current_order = max_order;
	for(i = 0; i < 256; i++)
		scoreboard[i] = 0;
}

void update_table(CONTEXT *table, int symbol)
{
	int i, index;
	unsigned char temp;
	CONTEXT *temp_ptr;
	unsigned int new_size;
	
	index = 0;
	while(index <= table -> max_index && table -> stats[index].symbol != (unsigned char)symbol)
		index++;
	if(index > table -> max_index)
	{
		table -> max_index++;
		new_size = sizeof(LINKS);
		new_size *= table -> max_index +1;
		if(current_order < max_order)
		{
			if(table -> max_index == 0)
				table -> links = (LINKS *) calloc(new_size, 1);
			else
				table -> links = (LINKS *) realloc((char *)table -> links, new_size);
			if(table -> links == NULL)
				fatal_error("Error #9: reallocating table space!");
			table -> links[index].next = NULL;
		}
		new_size = sizeof(STATS);
		new_size *= table -> max_index + 1;
		if(table -> max_index == 0)
			table -> stats = (STATS *) calloc(new_size, 1);
		else
			table -> stats = (STATS *) realloc((char *) table -> stats, new_size);
		if(table -> stats == NULL)
			fatal_error("Error #10: reallocating table space!");
		table -> stats[index].symbol = (unsigned char) symbol;
		table -> stats[index].counts = 0;
	}
	
	i = index;
	while(i > 0 && table -> stats[index].counts == table -> stats[i - 1].counts)
		i--;
	if(i != index)
	{
		temp = table -> stats[index].symbol;
		table -> stats[index].symbol = table -> stats[i].symbol;
		table -> stats[i].symbol = temp;
		if(table -> links != NULL)
		{
			temp_ptr = table -> links[index].next;
			table -> links[index].next = table -> links[i].next;
			table -> links[i].next = temp_ptr;
		}
		index = i;
	}
	
	table -> stats[index].counts++;
	if(table -> stats[index].counts == 255)
		rescale_table(table);
}


int convert_int_to_symbol(int c, SYMBOL *s)
{
	int i;
	CONTEXT *table;
	
	table = contexts[current_order];
	totalize_table(table);
	s -> scale = totals[0];
	if(current_order == -2)
		c = -c;
	for(i = 0; i <= table -> max_index; i++)
	{
		if(c == (int) table -> stats[i].symbol)
		{
			if(table -> stats[i].counts == 0)
				break;
			s -> low_count = totals[i + 2];
			s -> high_count = totals[i + 1];
			return (0);
		}
	}
	s -> low_count = totals[1];
	s -> high_count = totals[0];
	current_order--;
	return (1);
}

void get_symbol_scale(SYMBOL *s)
{
	CONTEXT *table;
	
	table = contexts[current_order];
	totalize_table(table);
	s -> scale = totals[0];
}

int convert_symbol_to_int(int count, SYMBOL *s)
{
	int c;
	CONTEXT *table;
	
	table = contexts[current_order];
	for(c = 0; count < totals[c]; c++);
	s -> high_count = totals[c - 1];
	s -> low_count = totals[c];
	if(c == 1)
	{
		current_order--;
		return (ESCAPE);
	}
	if(current_order < -1)
		return ((int) -table -> stats[c - 2].symbol);
	else
		return (table -> stats[c - 2].symbol);
}

void add_character_to_model(int c)
{
	int i;
	if(max_order < 0 || c < 0)
		return ;
	contexts[max_order] = shift_to_next_context(contexts[max_order], c, max_order);
	for(i = max_order - 1; i > 0; i--)
		contexts[i] = contexts[i + 1] -> lesser_context; 
}

CONTEXT* shift_to_next_context(CONTEXT *table, int c, int order)
{
	int i;
	CONTEXT *new_lesser;
	
	table = table -> lesser_context;
	if(order == 0)
		return (table -> links[0].next);
	for(i = 0; i <= table -> max_index; i++)
		if(table -> stats[i].symbol == (unsigned char)c)
		{
			if(table -> links[i].next != NULL)
				return (table -> links[i].next);
			else
				break;
		}
				
	new_lesser = shift_to_next_context(table, c, order - 1);
	
	table = allocate_next_order_table(table, c, new_lesser);
	return (table);
}

void rescale_table(CONTEXT *table)
{
	int i;
	
	if(table -> max_index == -1)
		return;
	for(i = 0; i <= table -> max_index; i++)
		table -> stats[i].counts /= 2;
	if(table -> stats[table -> max_index].counts == 0 && table -> links == NULL)
	{
		while(table -> stats[table -> max_index].counts == 0 && table -> max_index >= 0)
			table -> max_index --;
		if(table -> max_index == -1)
		{
			free((char *)table -> stats);
			table -> stats = NULL;
		}
		else
		{
			table -> stats = (STATS *) realloc((char *)table -> stats, sizeof(STATS) * (table -> max_index + 1));
			if(table -> stats == NULL)
				fatal_error("Error #11: reallocating stats space");
		}
	}
}

void totalize_table(CONTEXT *table)
{
	int i;
	unsigned char max;
	
	for( ; ; )
	{
		max = 0;
		i = table -> max_index + 2;
		totals[i] = 0; 
		for( ; i > 1; i--)
		{
			totals[i - 1] = totals[i];
			if(table -> stats[i - 2].counts)
				if((current_order == -2) || scoreboard[table -> stats[i - 2].symbol] == 0)
					totals[i - 1] += table -> stats[i - 2].counts;
			if(table -> stats[i - 2].counts > max)
				max = table -> stats[i - 2].counts;
		}
		
		if(max == 0)
			totals[0] = 1;
		else
		{
			totals[0] = (short int)(256 - table -> max_index);
			totals[0] *= table -> max_index;
			totals[0] /= 256;
			totals[0] /= max;
			totals[0] ++;
			totals[0] += totals[1];
		}
		if(totals[0] < MAXIMUM_SCALE)
			break;
		rescale_table(table);
	}
	for(i = 0; i < table -> max_index; i++)
		if(table -> stats[i].counts != 0)
			scoreboard[table -> stats[i].symbol] = 1;
}

void recursive_flush(CONTEXT *table)
{
	int i;
	
	if(table -> links != NULL)
		for(i = 0; i <= table -> max_index; i++)
			if(table -> links[i].next != NULL)
				recursive_flush(table -> links[i].next);
	rescale_table(table);
}

void flush_model()
{
	putc('F', stdout);
	recursive_flush(contexts[0]);
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
