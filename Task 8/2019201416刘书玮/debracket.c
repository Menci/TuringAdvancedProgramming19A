#include<stdlib.h>
#include<stdio.h>
#define K 4

struct tree{
	int left,right;
	char bas[K];
};
struct pair{
	int first;
	int second;
};

extern int get_val(char *,int);
struct pair debracket(char *_base, struct tree *_tree_base, struct pair now)
{
	int p = now.second;
	int f1 = get_val(_base, now.first);
	if(f1){
		int f2 = get_val(_base, now.first + 1);
		if(f2){
			(_tree_base + p)-> right = now.second + 1;
			now.first  += 2;
			now.second += 1;
			now = debracket(_base, _tree_base, now);
		}
		else{
			(_tree_base + p)-> left = now.second + 1;
			now.first  += 2;
			now.second += 1;
			now = debracket(_base, _tree_base, now);
		}
	}
	
	f1 = get_val(_base, now.first);
	if(f1){
		int f2 = get_val(_base, now.first + 1);
		if(f2){
			(_tree_base + p)-> right = now.second + 1;
			now.first  += 2;
			now.second += 1;
			now = debracket(_base, _tree_base, now);
		}
		else{
			(_tree_base + p)-> left = now.second + 1;
			now.first  += 2;
			now.second += 1;
			now = debracket(_base, _tree_base, now);
		}
	}
	
	f1 = get_val(_base, now.first);
	if(!f1){
		now.first  += 1;
		return now;
	}
}
