#include<stdio.h>
#define K 4

extern int get_val(char *,int);
struct huffman_node{
	struct huffman_node *left, *right;
	int count;
	int ord;
	int code, len;
};
struct tree{
	int left,right;
	char bas[K];
};
struct pair{
	int first;
	int second;
};
struct pair huffman_dfs(char *_base, char *_out_base, struct huffman_node *p, struct pair now)
{	
	if(now.second == 0) return now;
	puts("1");
	if(p-> count){
		int i;
		for(i = 0; i < 8 * K; i += 1){
			*(_out_base + now.second + i / 8) *= 2;
			*(_out_base + now.second + i / 8) += get_val(_base, now.first + i);
		}
		now.second += K;
		now.first  += 8 * K;
		return now;
	}
	int f = get_val(_base, now.first);
	now.first += 1;
	if(f){
		return huffman_dfs(_base, _out_base, p-> right, now);
	}
	else{
		return huffman_dfs(_base, _out_base, p-> left,  now);
	}
}
int tree_dfs(char *_out_base, struct tree *root, int p, int place)
{
	if((root + p)-> left){
		place = tree_dfs(_out_base, root, (root + p)-> left, place);
	}
	int i;
	for(i = 0; i < K; i += 1){
		(root + p)-> bas[i] = *(_out_base + place + i);
	}
	place += K;
	if((root + p)-> right){
		place = tree_dfs(_out_base, root, (root + p)-> right, place);
	}
	return place;
}
