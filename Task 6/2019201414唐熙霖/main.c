#include <stdio.h>
#include <stdlib.h>

extern void solve(int n, int k, int type);

void syntax_error()
{
	puts("Syntax error!");
	puts("");
	puts("Input Format: [1-9][0-9]{0,8} [1-9][0-9]{0,8} [0-1]{0,1}");
}

int main(int argc, char *argv[])
{
	int n, k, type = 0;

	switch(argc)
	{
		case 4:
			type = atoi(argv[3]);
		case 3:
			n = atoi(argv[1]),
			k = atoi(argv[2]);
			if(n<0 || k<0 || (type!=0 && type!=1))
				return syntax_error(), 0;

			solve(n, k-1, type);
			break;

		default:
			syntax_error();
			break;
	}
	return 0;
}
