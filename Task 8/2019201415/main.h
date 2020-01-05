void CompressFile(FILE *input, BIT_FILE *output, int argc, char *argv[]);
void ExpandFile(BIT_FILE *input, FILE *output, int argc, char *argv[]);

extern char *Usage;
extern char *CompressionName;

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
