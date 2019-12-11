#include "binary_search_tree.h"

extern node *rt;

int main()
{
	if(fopen("in.txt","r"))
	{
		freopen("in.txt","r",stdin);
		freopen("out.txt","w",stdout);
	}

	int n = read(), i;
	for(i = 1; i <= n; i++)
	{
		switch(read())
		{
			case 0:rt = insert(read()); break;
			case 1:rt = dec(read()); break;
			case 2:writes(findkth(read())); break;
			case 3:writes(findrk(read())); break;
			case 4:writes(query(read(), 0)); break;
			case 5:writes(query(read(), 1)); break;
		}
	}

	freeall(rt);
	return 0;
}
