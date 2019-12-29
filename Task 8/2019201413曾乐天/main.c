#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#ifndef K
#define K 8
#endif

#define N 100000000
#define OFFSET 128 
#define MAX_CHAR 256
#define MAX_POOL 100000000

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct xxx
{
	int lc, rc;
	char data[K];
}node;

typedef struct yyy
{
	int fa, lc, rc;
}huff;

typedef struct zzz
{
	int val, key;
}hnode;

int read(char *s)
{
	int x = 0, c = 0;;
	while(s[c] < '0' || '9' < s[c])c++;
	while('0' <= s[c] && s[c] <= '9')x = x * 10 + (s[c++] ^ 48);
	return x;
}

void mswap(hnode *a, hnode *b)
{
	hnode c = *a;
	*a = *b;
	*b = c;
}

hnode heap[N];
int cnt, hsize = sizeof(hnode);

void push(int x, int c)
{
    heap[++cnt] = (hnode){x, c};
    int now = cnt;
    while(now > 1)
	{
        int nxt = now >> 1;
        if(heap[nxt].val >heap[now].val)
			mswap(&heap[nxt], &heap[now]);
        else break; 
        now = nxt;
    }
}

void pop()
{
    mswap(&heap[cnt], &heap[1]);
	cnt--;
    int now = 1;
    while((now << 1) <= cnt)
	{
        int nxt = now << 1;
        if(nxt + 1 <= cnt && heap[nxt + 1].val < heap[nxt].val)
			nxt++;
        if(heap[nxt].val < heap[now].val)
			mswap(&heap[now], &heap[nxt]);
        else break;
        now = nxt;
    }
}

int mp[N], otmp;
int buc[3000], hpool, *tmp, nn, bpool;
int n, deg[N], root, cpos, stkp;
uchar buffer[MAX_POOL];
huff ht[3000];
node *a;

void outbit(uchar c)
{
	int i;
	for(i = 7; ~i; i--)
		putchar('0' + ((c >> i) & 1));
	putchar('\n');
}

void wbit(int pos, int val)
{
	if(val == 0)
		buffer[pos / 8] &= (((uchar)((1 << 8) - 1)) ^ (1 << (pos % 8)));
	else
		buffer[pos / 8] |= 1 << (pos % 8);
}

int qbit(int pos)
{
	return (buffer[pos / 8] >> (pos % 8))&1;
}

void write(int x, int v)
{
	if(~ht[x].fa)
		write(ht[x].fa, ht[ht[x].fa].rc == x);
	if(~v)
		wbit(bpool++, v);
}

void print(node x, int id)
{
	if(~id)
		printf("%d:", id);
	printf("[%d][%d]\n", x.lc, x.rc);
}

void dfswrite1(int x)
{
	mp[x] = otmp++;
	wbit(bpool++, a[x].lc != -1);
	wbit(bpool++, a[x].rc != -1);

	if(~a[x].lc)
		dfswrite1(a[x].lc);
	if(~a[x].rc)
		dfswrite1(a[x].rc);
}

void dfswrite2(int x)
{
	int j;

	printf("%d:[%d][%d]\n", mp[x], ~a[x].lc ? mp[a[x].lc] : a[x].lc, ~a[x].rc ? mp[a[x].rc] : a[x].rc);
	for(j = 0; j < K; j++)
		printf("%d ", a[x].data[j]);
	puts("");
	
	for(j = 0; j < K; j++)
		write(buc[a[x].data[j] + OFFSET], -1);
	if(~a[x].lc)
		dfswrite2(a[x].lc);
	if(~a[x].rc)
		dfswrite2(a[x].rc);
}

void compress()
{
	int size, i, j;

	freopen("com_log.txt","w",stdout);
	FILE* input = fopen("input.bin", "rb");
	FILE* output = fopen("zipped.bin", "wb");
	
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);

	n = size / sizeof(node);
	a = malloc(size + sizeof(node) * 10);
	fread(a, sizeof(node), n, input);
	
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < K; j++)
			buc[a[i].data[j] + OFFSET]++;
		if(0 <= a[i].lc && a[i].lc < n)
			deg[a[i].lc]++;
		if(0 <= a[i].rc && a[i].rc < n)
			deg[a[i].rc]++;
	}

	for(i = 0, root = -1; i < n && !(~root); i++)
		if(deg[i] == 0)
			root = i;

	hpool = -1;
	for(i = 0; i < MAX_CHAR; i++)
		if(buc[i])
		{
			ht[++hpool] = (huff){-1, -1, i};
			push(buc[i], hpool);
			buc[i] = hpool;
		}

	while(cnt >= 2)
	{
		hnode n1 = heap[1];
		pop();
		hnode n2 = heap[1];
		pop();

		ht[++hpool] = (huff){-1, n1.key, n2.key};
		ht[n1.key].fa = hpool;
		ht[n2.key].fa = hpool;
		push(n1.val + n2.val, hpool);
	}
	
	//output huffman
	int *tmp = malloc(sizeof(int) * 3);
	tmp[0] = hpool;
	tmp[1] = n;
	fwrite(tmp, sizeof(int), 2, output);
	fwrite(ht, sizeof(huff), hpool + 1, output);
	free(tmp);

	//output compressed

	dfswrite1(root);
	dfswrite2(root);

	fwrite(buffer, sizeof(char), (bpool + 8)/8, output);
	free(a);
	fflush(output);
}

char query(int x)
{
	if(ht[x].lc == -1)
		return (char)(ht[x].rc - OFFSET);
	else if(qbit(cpos++) == 1)
		return query(ht[x].rc);
	else
		return query(ht[x].lc);
}

int dfs_build(int x)
{
	int i, j;

	for(i = 0; i < K; i++)
		a[x].data[i] = query(hpool);
	a[x].lc = a[x].rc = -1;

	int lc = qbit(stkp++);
	int rc = qbit(stkp++);
	if(lc)
		a[x].lc = dfs_build(++nn);
	if(rc)
		a[x].rc = dfs_build(++nn);
	return x;
}

void decompress()
{
	int tmp[2], i, j, cntsiz;
	freopen("dec_log.txt", "w", stdout);
	FILE* input = fopen("zipped.bin", "rb");
	FILE* output = fopen("output.bin", "wb");

	fread(tmp, sizeof(int), 2, input);
	hpool = tmp[0];
	n = tmp[1];

	fread(ht, sizeof(huff), hpool + 1, input);
	bpool = fread(buffer, 1, MAX_POOL - 1, input);
	a = malloc(sizeof(node) * (n + 10));

	cpos = n * 2;
	stkp = 0;
	dfs_build(nn = 0);

	fwrite(a, sizeof(node), n, output);

	for(i = 0; i < n; i++)
	{
		print(a[i], i);
		for(j = 0; j < K; j++)
			printf("%d ", a[i].data[j]);
		puts("");
	}
}

int main(int argc, char **argv)
{
	if(argc == 2)
	{
		int op = read(argv[1]);
		if(op == 0)
			compress();
		else if(op == 1)
			decompress();
		else
			printf("ERROR");
	}
	else
	{
		compress();
		decompress();
	}

	return 0;
}
