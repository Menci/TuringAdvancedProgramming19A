#include"main.h"

int main(int argv, char* arg[])
{
    int len_of_loop, len_of_gap;
	unsigned char anti;

	if(!check_cmd(argv, arg))
		return 0;

	sscanf(arg[1],"%d",&len_of_loop);
	if(arg[2][0]=='a')
		anti = 1;
	else
		anti = 0;
	sscanf(arg[3],"%d",&len_of_gap);

	solve(len_of_loop, anti, len_of_gap);
}

static void solve(int len_of_loop, unsigned char anti, int len_of_gap)
{
    LN *now = build(len_of_loop), *to;
	if(anti)
    	while(len_of_loop--)
    	{
        	to = goto_node_anti(now, len_of_gap);
        	now = to->pre;
        	print_node(to);
        	del_node(to);
    	}
	else
		while(len_of_loop--)
    	{
        	to = goto_node_wise(now, len_of_gap);
        	now = to->nxt;
        	print_node(to);
        	del_node(to);
    	}
	
}

