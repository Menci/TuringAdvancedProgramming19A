#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h> 
#include "errhand.h"

char *Usage = "***.in out-file(must have ***.cc with ***.in)\n";

void usage_exit(char *prog_name)
{
	char *short_name;
	char *extension;
	
	short_name = strrchr(prog_name, '\\');
	if(short_name == NULL)
		short_name = strrchr(prog_name, '/');
	if(short_name == NULL)
		short_name = strrchr(prog_name, ':');
	if(short_name != NULL)
		short_name++;
	else
		short_name = prog_name;
	extension = strrchr(short_name, '.');
	if(extension != NULL)
		*extension = '\0';
	printf("\nUsage: %s %s\n", short_name, Usage);
	exit(0);
}

#ifndef SEEK_END 
#define SEEK_END 2
#endif

long file_size(char *name)
{
	long eof_ftell;
	FILE *file;
	
	file = fopen(name, "r");
	if(file == NULL)
		return (0L);
	fseek(file, 0L, SEEK_END);
	eof_ftell = ftell(file);
	fclose(file);
	return (eof_ftell);
}

void print_ratios(char *input, char *output)
{
	long input_size;
	long output_size;
	double ratio;
	
	input_size = file_size(input);
	if(input_size == 0)
		input_size = 1;
	output_size = file_size(output);
	ratio = (output_size * 100.0 / input_size);
	printf("\nInput bytes:    %ld\n", input_size);
	printf("Output bytes:    %ld\n", output_size);
	
	printf("Total Compression ratio: \033[1;32;40m %.4lf%% \033[0m \n", ratio);
}

int cmpsize(char *input, char* output)
{
	return file_size(input) > file_size(output);
}

void syscmd(char *fmt, ...)
{
	va_list argptr;
	
	va_start(argptr, fmt);
	char *s = malloc(1024);
	vsprintf(s, fmt, argptr);
	puts(s);
	if(system(s) != 0)
		fatal_error("System Error\n");
	va_end(argptr);
}

int main(int argc, char* argv[])
{
	if(argc < 3)
		usage_exit(argv[0]);
	
	// Reading N & K && Making
	int len = strlen(argv[1]);
	char* s = malloc(len + 1), *t = malloc(len + 1);
	memcpy(s, argv[1], len+1);
	memcpy(t, argv[1], len+1);
	s[len - 1] = s[len - 2] = 'c'; 
	FILE* input = fopen(s, "r");
	if(input == NULL)
		fatal_error("Error opening %s for N and K\n", s);
	int N, K, lzw = 1;
	if(fscanf(input, "#define K %d\n", &K) != 1)
		fatal_error("Error reading K from %s\n", s);
	if(fscanf(input, "#define N %d\n", &N) != 1)
		fatal_error("Error reading N from %s\n", s);
	syscmd("make tree lzw arith N=%d K=%d", N, K);
	printf("\033[1;32;40m Making Success!\033[0m\n");
	
	//Compressing Using TreeStruction -> LZW -> Arithmetic
	s[len - 2] = 't'; s[len - 1] = 's';
	t[len - 2] = 'l'; t[len - 1] = 'z';
	syscmd("/tmp/tree-c %s %s", argv[1], s);
	syscmd("/tmp/lzw-c %s %s", s, t);
	if(!cmpsize(s, t))
	{
		lzw = 0;
		syscmd("cp %s %s", s, t);
	}
	syscmd("/tmp/arith-c %s %s", t, argv[2]);
	syscmd("rm -rf %s %s", s, t);
	printf("\033[1;32;40m Compression Success!\033[0m\n");
	
	//Expanding
	syscmd("/tmp/arith-e %s %s", argv[2], t);
	if(lzw)
		syscmd("/tmp/lzw-e %s %s", t, s);
	else
		syscmd("cp %s %s", t, s);
	t[len - 2] = 'o'; t[len - 1] = 't';
	syscmd("/tmp/tree-e %s %s", s, t);
	printf("\033[1;32;40m Expandion Success!\033[0m\n");
	print_ratios(argv[1], argv[2]);
	
	//Checking the struction and data of the tree by TreeStruction compressing  
	printf("Checking\n");
	s[len - 2] = 't'; s[len - 1] = '1';
	syscmd("/tmp/tree-c %s %s", argv[1], s);
	s[len - 1] = '2';
	syscmd("/tmp/tree-c %s %s", t, s);
	syscmd("rm -rf %s", t);
	t[len - 2] = 't'; t[len - 1] = '1';
	syscmd("diff %s %s", t, s);
	puts("\033[1;32;40m Right! \033[0m");
	syscmd("rm %s %s", t, s); 
	print_ratios(argv[1], argv[2]);
}
