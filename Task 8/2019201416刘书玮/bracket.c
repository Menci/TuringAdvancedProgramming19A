#include<stdlib.h>
#include<string.h>
#include<stdio.h>	
#define K 8

extern void _add(char*, int, int);

struct tree{
	int left, right;
	char bas[K];
};
struct pair{
	int first;
	int second;
};
struct pair bracket_dfs(struct tree *_base,int _num, char* _out_base, char* _data_base, int cnt, int vis, int k)
{
	struct tree *_place = _base + _num;
	memcpy(_data_base + vis * k, &(_place-> bas) , K);
	vis ++;
	if(_place-> left != -1){
		_add(_out_base, cnt ++, 1);
		_add(_out_base, cnt ++, 0);
		struct pair save = bracket_dfs(_base, _place-> left, _out_base, _data_base, cnt, vis, k);
		cnt = save.first;
		vis = save.second;
	}
	if(_place-> right != -1){
		_add(_out_base, cnt ++, 1);
		_add(_out_base, cnt ++, 1);
		struct pair save = bracket_dfs(_base, _place-> right, _out_base, _data_base, cnt, vis, k);
		cnt = save.first;
		vis = save.second;
	}
	_add(_out_base, cnt ++, 0);
	struct pair ret;
	ret.first  = cnt;
	ret.second = vis;
	return ret;
}
