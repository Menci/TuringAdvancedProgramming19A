#include"unzip_tree.h"

static int get_len()
{
	int len;
	FILE *input = fopen("unzipped.bin", "rb");
	fseek(input, 0, SEEK_END);
	len = ftell(input);
	fclose(input);
	return len;
}

static void dfs(int i)
{
	if(!is_node(i))
		return;
	fread(t_node[i].data, sizeof(char), _K, input);

	if(son[i]&1)
		t_node[i].lc = ++tot;
	else
		t_node[i].lc = -1;
	dfs(t_node[i].lc);

	if(son[i]&2)
		t_node[i].rc = ++tot;
	else
		t_node[i].rc = -1;
	dfs(t_node[i].rc);
}

static void print(int i)
{
	if(!is_node(i))
		return;
	print(t_node[i].lc);
	fwrite(t_node[i].data, sizeof(char), _K, output);
	print(t_node[i].rc);
}

void unzip_tree()
{
	memset(t_node, 0, sizeof(t_node));
	int len = get_len();
	input = fopen("unzipped.bin", "rb");
	output = fopen("unzippeded.bin", "wb");
	len <<= 2;
	len /= (4 * _K + 1);
	fseek(input, _K * len, SEEK_SET);
	char tmp;
	for(int i=1;i<=len;i++)
	{
		fread(&tmp, sizeof(char), 1, input);
		for(int i=0;i<=3;i++)
			son[tot++] = (tmp>>(i*2))&3;
	}
	fseek(input, 0, SEEK_SET);
	tot=0;
	dfs(0);
	print(0);

	fclose(input);
	fclose(output);
}

