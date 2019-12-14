// main.c

#include <math.h>
#include "Scapegoat_tree.h"

inline int read()
{
	int x = 0, f = 1;
	char ch = getchar();
	while (ch < '0' || ch > '9')
	{
		if (ch == '-')
			f = -1;
		ch = getchar();
	}
	while (ch <= '9' && ch >= '0')
	{
		x = (x<<1) + (x<<3) + ch - '0';
		ch = getchar();
	}
	return x * f;
}

void print(int x)
{
	if (abs(x) == inf)
		return (void)(putchar('-'), putchar('1'));
	if (x < 0)
		putchar('-'), x = -x;
	if (x >= 10)
		print(x / 10);
	putchar(x % 10 + '0');
}

int main()
{
	int n, opt, x;
	spg *tree = initial();
	spgstk *stk = new_stk();

	scanf("%d", &n);
	while (n--)
	{
		scanf("%d%d", &opt, &x);
		switch (opt)
		{
		case 0:
			insert(tree, stk, x);
			break;
		case 1:
			del(tree, get_pos(tree, x));
			break;
		case 2:
			print(query_kth(tree, x + 1));
			puts("");
			break;
		case 3:
			print(query_rank(tree, x) - 1);
			puts("");
			break;
		case 4:
			print(query_pre(tree, x));
			puts("");
			break;
		case 5:
			print(query_sub(tree, x));
			puts("");
			break;
		}
	}
	termination(tree, stk);
	return 0;
}
