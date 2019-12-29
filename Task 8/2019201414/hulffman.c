#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic.h"
#include "hulffman.h"
#include "bracket_sequence.h"

#define min(a,b) (a)<(b) ? (a) : (b)

int cmp(const void *a, const void *b)
{
	return (*(code_table *)a).w - (*(code_table *)b).w;
}

void dfs_hulffman(int *trace, int *buffer, int dep, code_table *table, int u)
{
	if (table[u].ls == -1)
	{
		memcpy(trace + table[u].val*300, buffer, sizeof(int) * (1+dep));
		return ;
	}
	buffer[++buffer[0]] = 0;
	dfs_hulffman(trace, buffer, dep+1, table, table[u].ls);
	buffer[buffer[0]] = 1;
	dfs_hulffman(trace, buffer, dep+1, table, table[u].rs);
	buffer[0]--;
}

int dfs_tree(node *tree, int u, int *trace, uchar *compressed, int bit)
{
	int i, j;
	for (i = 0; i < K; ++i)
	{
//printf("%d %d\n", tree[u].w[i], tree[u].w[i]);
		for (j = 1; j <= trace[tree[u].w[i]*300]; ++j)
			fill(compressed, bit, trace[tree[u].w[i]*300 + j]), bit++;//,printf("%d ",bit);
	}

	if (~tree[u].ls)
		bit = dfs_tree(tree, tree[u].ls, trace, compressed, bit);
	if (~tree[u].rs)
		bit = dfs_tree(tree, tree[u].rs, trace, compressed, bit);
	return bit;
}

void hulffman_encode(node *tree, int root, size_t n, FILE *output)
{
	code_table *buc = (code_table *)calloc(300, sizeof(code_table));
	int i, j;
	for (i = 0; i < n; ++i)
		for (j = 0; j < K; ++j)
			++buc[tree[i].w[j]].w,
			buc[tree[i].w[j]].val = tree[i].w[j],
			buc[tree[i].w[j]].ls = buc[tree[i].w[j]].rs = -1;
	
	qsort(buc, 256, sizeof(code_table), cmp);
	
	int head[2], tail[2];
	code_table *que[2];
	que[1] = (code_table *)malloc(sizeof(code_table) * 300);

	for (i = 0; i < 256; ++i)
		if (buc[i].w != 0)
		{
			que[0] = buc + i;
			tail[0] = 256 - i;
			break;
		}
	
	tail[1] = 1, head[0] = head[1] = 0;

	code_table *table = (code_table *)malloc(sizeof(code_table)*1000);
	int now = 0;
	while (head[0] != tail[0] || head[1] != tail[1] - 1)
	{
		int fi, se, th, tmp;
		if (tail[0] - head[0] > 1)
		{
			if (tail[1] - head[1] > 1)	
			{
				fi = que[0][head[0]].w + que[0][head[0] + 1].w;
				se = que[1][head[1]].w + que[1][head[1] + 1].w;
				th = que[0][head[0]].w + que[1][head[1]].w;

				tmp = min(min(fi, se), th);
				if(fi == tmp)
				{
					table[now++] = que[0][head[0]];
					table[now++] = que[0][head[0]+1];
					head[0] += 2;
					que[1][tail[1]].w = fi;
					que[1][tail[1]].ls = now-2;
					que[1][tail[1]++].rs = now-1;
				}
				else if (se == tmp)
				{
					table[now++] = que[1][head[1]];
					table[now++] = que[1][head[1]+1];
					head[1] += 2;
					que[1][tail[1]].w = se;
					que[1][tail[1]].ls = now-2;
					que[1][tail[1]++].rs = now-1;
				}
				else 
				{
					table[now++] = que[0][head[0]];
					table[now++] = que[1][head[1]];
					++head[0], ++head[1];
					que[1][tail[1]].w = th;
					que[1][tail[1]].ls = now-2;
					que[1][tail[1]++].rs = now-1;
				}
			}

			else if (tail[1] - head[1] == 1)
			{//puts("SDFSDFSD\n");
				fi = que[0][head[0]].w + que[0][head[0] + 1].w;
				se = que[0][head[0]].w + que[1][head[1]].w;

				tmp = min(fi, se);
				if(fi == tmp)
				{
					table[now++] = que[0][head[0]];
					table[now++] = que[0][head[0]+1];
					head[0] += 2;
					que[1][tail[1]].w = fi;
					que[1][tail[1]].ls = now-2;
					que[1][tail[1]++].rs = now-1;
				}
				else 
				{
					table[now++] = que[0][head[0]];
					table[now++] = que[1][head[1]];
					++head[0], ++head[1];
					que[1][tail[1]].w = se;
					que[1][tail[1]].ls = now-2;
					que[1][tail[1]++].rs = now-1;
				}
			}

			else
			{
				table[now++] = que[1][head[1]];
				table[now++] = que[1][head[1]+1];
				head[1] += 2;
				que[1][tail[1]].w = se;
				que[1][tail[1]].ls = now-2;
				que[1][tail[1]++].rs = now-1;
			}
		}

		else if (tail[0] - head[0] == 1)
		{
			if (tail[1] - head[1] > 1)	
			{
				se = que[1][head[1]].w + que[1][head[1] + 1].w;
				th = que[0][head[0]].w + que[1][head[1]].w;

				tmp = min(se, th);
				if (se == tmp)
				{
					table[now++] = que[1][head[1]];
					table[now++] = que[1][head[1]+1];
					head[1] += 2;
					que[1][tail[1]].w = se;
					que[1][tail[1]].ls = now-2;
					que[1][tail[1]++].rs = now-1;
				}
				else 
				{
					table[now++] = que[0][head[0]];
					table[now++] = que[1][head[1]];
					++head[0], ++head[1];
					que[1][tail[1]].w = th;
					que[1][tail[1]].ls = now-2;
					que[1][tail[1]++].rs = now-1;
				}	
			}

			else
			{
				se = que[0][head[0]].w + que[1][head[1]].w;

				table[now++] = que[0][head[0]];
				table[now++] = que[1][head[1]];
				++head[0], ++head[1];
				que[1][tail[1]].w = se;
				que[1][tail[1]].ls = now-2;
				que[1][tail[1]++].rs = now-1;
			}
		}

		else
		{
			fi = que[1][head[1]].w + que[1][head[1] + 1].w;
			table[now++] = que[1][head[1]];
			table[now++] = que[1][head[1]+1];
			head[1] += 2;
			que[1][tail[1]].w = fi;
			que[1][tail[1]].ls = now-2;
			que[1][tail[1]++].rs = now-1;
		}
	}
	
	table[now] = que[1][head[1]];
	int hroot = now + 1;

	fwrite(&hroot, sizeof(int), 1, output);
	fwrite(table, sizeof(code_table), now+1, output);
	hroot = now;

	int *trace = (int *)calloc(90000, sizeof(int));
	int *buffer = (int *)calloc(300, sizeof(int));

	dfs_hulffman(trace, buffer, 0, table, hroot);
	
	uchar *compressed = (uchar *)malloc(sizeof(int)*n*K);
	int end = dfs_tree(tree, root, trace, compressed, 0) - 1;

	fwrite(&end, sizeof(int), 1, output);
	fwrite(compressed, sizeof(uchar), (end>>3) + 1, output);

	free(buc), free(table), free(trace), free(compressed);
}

void hulffman_decode(int code_num, code_table *table, FILE *compressed, FILE *output)
{
	int n;
	fread(&n, sizeof(int), 1, compressed);
	uchar *buffer = (uchar *)malloc(sizeof(uchar) * n);
	fread(buffer, sizeof(uchar), (n>>3) + 1, compressed);

	int i = 0, now ;
	for (i = 0; i < n; ++i)
	{
		now = read(buffer, i);
		int HTcur, cur;
		int HTcurweight, HTcurlchild, HTcurchild;
		char codecode_len;
		if (HTcurweight == 0)
		{
			HTcurweight = 1;
			if (HTcurlchild != -1)
			{  
				codecode_len = '0';
				cur = HTcurlchild;
			}
			else
			{
				codecode_len = '\0';
				if (HTcur)
				{
					if (HTcurlchild != -1)
					{  
						codecode_len = '0';
						cur = HTcurlchild;
					}
				}
			}
		}
	}

	fwrite(buffer, sizeof(uchar), (n>>3) + 1, output);
}
