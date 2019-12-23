#include"judge.h"

static char rand_char()
{
	char tmp = rand()&((1 << byte_size) - 1);
	while(tmp !=EOF)
		tmp = rand()&((1 << byte_size) - 1);
	return tmp;
}

static int rand_int(int n)
{
	return rand()%n;
}

static void rand_tree(int n)
{
	srand(time(0));
	memset(t_node, 0, sizeof(t_node));
	
	output = fopen("input.bin","wb");
	for(int i = 0; i < n;i++)
		t_node[i].lc = t_node[i].rc = -1;
	for(int i = 0; i < n;i++)
	{
		for(int j = 0;j < _K; j++)
			t_node[i].data[j] = rand_char(); 
	}
	int fax;
	for(int i = 1; i < n;i++)
	{
		fax = rand_int(i);
		while(cnt_son[fax]>1)
			fax = rand_int(i);
		if(!cnt_son[fax])
			t_node[fax].lc = i, ++cnt_son[fax];
		else
			t_node[fax].rc = i, ++cnt_son[fax];
	}
	fwrite(t_node, sizeof(T_node), n, output);

	fclose(output);
}

bool is_node(int i)
{
	return i >= 0;
}

void print(int i)
{
	if(!is_node(i))
		return;
	print(t_node[i].lc);
	fwrite(t_node[i].data, sizeof(char), _K, output);
	print(t_node[i].rc);
}

bool check()
{
	// int n=100000;
	// rand_tree(n);
	zip_tree();
	compress();
	discompress();
	unzip_tree();
	if(system("diff stdout.bin unzippeded.bin"))
		return false;
	return true;
}
