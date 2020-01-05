#include "lzw.h"

static int get_len_cp()
{
	int len;
	FILE *input = fopen("zipped.bin", "rb");
	fseek(input, 0, SEEK_END);
	len = ftell(input);
	fclose(input);
	return len;
}

static int get_len_dp()
{
	int len;
	FILE *input = fopen("zippeded.bin", "rb");
	fseek(input, 0, SEEK_END);
	len = ftell(input);
	fclose(input);
	return len;
}

static void print(char* x)
{
	fwrite(x, sizeof(char), 3, output);
}

static void read(char *x)
{
	fread(x, sizeof(char), 3, input);
}

static void build_tree()
{
	for(int i = 0;i <= byte_max; i ++)
		my_treap[i] = Treap_build(comp, sizeof(TR_node));
}

static void init_cp()
{					
	build_tree();
	for(int i = 0;i <= byte_max; i++)
	{
		trie_node[i].pre = -1;
		trie_node[i].c = i;
		trie_node[i].num = i;
		Treap_insert(my_treap[i], &trie_node[i]);
	}
	cnt = byte_max + 1;
	P = -1;
}

static void init_dp()
{					
	for(int i = 0;i <= byte_max; i ++)
	{
		trie_node[i].pre = -1;
		trie_node[i].c = i;
		trie_node[i].num = i;
	}
	cnt = byte_max + 1;
	P = -1;
}

static void out(int x)
{                            
    if(trie_node[x].pre != -1)
        out(trie_node[x].pre);
    else 
        V = trie_node[x].c;
	fwrite(&trie_node[x].c, sizeof(char), 1, output);
}

static void search()
{
	TR_node tool,*tmp;
    int flag = 0;
	tool.pre = P;
	tmp = Treap_find(my_treap[W], &tool);
	if(tmp->pre == P)
	{
		flag = 1;
		P = tmp->num;
	}
	if(!flag)
	{
        trie_node[cnt].pre = P;
        trie_node[cnt].c = W;
		trie_node[cnt].num = cnt;
		Treap_insert(my_treap[W], &trie_node[cnt]);
		print((char*)&P);
        P = (int)W;
        cnt ++;
    }
}

static void research(){
    out(Q);
    if(P != -1)
	{
        trie_node[cnt].pre = P;
        trie_node[cnt].c = V;
		trie_node[cnt].num = cnt;
        cnt ++;
    }
}

static int comp(const void *a,const void *b)
{
	return ((TR_node*)a)->pre - ((TR_node*)b)->pre;
}

void compress()
{
	init_cp();
	int len = get_len_cp();
	input = fopen("zipped.bin","rb");
	output = fopen("zippeded.bin","wb");
	for(int i=0;i<len;i++)
		fread(&W, sizeof(char), 1, input),search();
	print((char*)&P);
	fclose(input);
	fclose(output);
}

void discompress()
{
    init_dp();
	int len = get_len_dp();
    input = fopen("zippeded.bin","rb");
	output = fopen("unzipped.bin","wb");
	for(int i = 1;i<=len;i+=3)
		read((char*)&Q),research();
	fclose(input);
	fclose(output);
}
