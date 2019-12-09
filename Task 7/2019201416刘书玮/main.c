#include<stdio.h>
#include<stdlib.h>
#include"splay.h"

#include<malloc.h>

int main(){
	int n,i,a,b;
	struct _splay *T = _splay_build();
	struct _node  *tmp = NULL;
	scanf("%d",&n);
	for(i = 0;i < n; i++){
		scanf("%d%d",&a,&b);
		switch(a){
			case 0:
				_splay_insert(T, b);
				break;
			case 1:
				_splay_erase(T, b);
				break;
			case 2:
				printf("%d\n",_splay_kth(T, b));
				break;
			case 3:
				printf("%d\n",_splay_rank(T, b));
				break;
			case 4:
				_splay_insert(T, b);
				tmp = _splay_pre(T);
				if(tmp == NULL) puts("-1");
				else printf("%d\n", tmp-> value);
				_splay_erase(T, b);
				break;
			case 5:
				_splay_insert(T, b);
				tmp = _splay_nex(T);
				if(tmp == NULL) puts("-1");
				else printf("%d\n", tmp-> value);
				_splay_erase(T, b);
				break;
		}
	}
	_splay_destory(T);
	return 0;
}
