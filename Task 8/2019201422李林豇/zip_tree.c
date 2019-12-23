#include"zip_tree.h"

static int get_len()
{
	int len;
	FILE *input = fopen("input.bin", "rb");
	fseek(input, 0, SEEK_END);
	len = ftell(input);
	fclose(input);
	return len;
}

static void dfs(int i)
{
	if(!is_node(i))
		return;
	in[++tot] = i;
	dfs(t_node[i].lc);
	dfs(t_node[i].rc);
}

// static void debug_print(int i)
// {
// 	if(!is_node(i))
// 		return;
// 	for(int j = 0; j<_K;j++)
// 		putchar(t_node[i].data[j]);
// 	printf(" %d %d\n", t_node[i].lc, t_node[i].rc);
// 	debug_print(t_node[i].lc);
// 	debug_print(t_node[i].rc);
// }

// static void debug()
// {
// 	freopen("zip.out","w",stdout);
// 	debug_print(0);
// 	fclose(stdout);
// }

void zip_tree()
{
	memset(t_node, 0, sizeof(t_node));
	int len = get_len();
	input = fopen("input.bin", "rb");
	output = fopen("zipped.bin", "wb");

	fread(t_node, sizeof(char), len, input);

	dfs(0);

	int per = byte_size / 2;
	for(int i = 1; i<=tot; i++)
		fwrite(t_node[in[i]].data, sizeof(char), _K, output);
	for(int i = 1; i <= tot; i+=per)
	{
		char tmp = 0;
		for(int j = 0; j < per;j++)
			tmp |= (is_node(t_node[in[i+j]].lc)<<(j<<1)) | (is_node(t_node[in[i+j]].rc)<<(j<<1|1));
		fwrite(&tmp, sizeof(char), 1, output);
	}

	fclose(input);
	fclose(output);
	
	output = fopen("stdout.bin","wb");
	print(0);
	fclose(output);

	// debug();-1814956065 -1814956623
	
}

