#include <ctype.h>
#include <stdio.h>
#include <stdint.h>  // uint64_t
#include <stdbool.h> // bool
#define N 102 
#define exp_num 60
#define var_num 60
typedef long long ll; 
void swap(int *a, int *b) {
	int t = *a; *a = *b; *b = t;
	return;
}
void swapll(ll *a, ll *b) {
	ll t = *a; *a = *b; *b = t;
	return;
}
ll num[N][3][2];
char op[N];
void init() {
	int i;
	for(i = 1; i <= exp_num; ++i) {
		char ch = 0; ll A1 = 0;
		while(!isalpha(ch)) ch = getchar();
		ch = getchar();
		while(isdigit(ch)) A1 = (A1 << 3) + (A1 << 1) + (ch ^ 48), ch = getchar();
		num[i][0][0] = 2; num[i][0][1] = A1;
		ll A2 = 0, flag = 2; 
		while(!isalpha(ch)) ch = getchar();
		if(ch == 'a') flag = 1; ch = getchar();
		while(isdigit(ch)) A2 = (A2 << 3) + (A2 << 1) + (ch ^ 48), ch = getchar();
		num[i][1][0] = flag; num[i][1][1] = A2;
		op[i] = getchar(); getchar(); ch = getchar();
		ll A3 = 0, w = 0; flag = 0;
		if(ch == 'a') flag = 1, ch = getchar();
		else if(isalpha(ch)) flag = 2, ch = getchar();
		else if(ch == '-') w |= (ch == '-'), ch = getchar();
		while(isdigit(ch)) A3 = (A3 << 3) + (A3 << 1) + (ch ^ 48), ch = getchar();
		num[i][2][0] = flag; num[i][2][1] = (w ? -A3 : A3);
	}
	return;
}
int lst[N], to_v[N], to_t[N], vis_v[N], vis_t[N];
void clear(int i) {
	int j;
	for(j = 0; j < 3; ++j) {
		int k = num[i][j][1];
		if(num[i][j][0] == 0) continue;
		if(num[i][j][0] == 1) k += var_num;
		if(lst[k] == i) {
			vis_v[to_v[k]] = 0; vis_t[to_t[k]] = 0;
			to_v[k] = 0; to_t[k] = 0;
		}
	}
	return;
}
int add(int i) {
	int j;
	for(j = 1; j <= 15; ++j) {
		if(!vis_t[j]) {
			vis_t[j] = 1;
			to_t[i] = j;
			return 0;
		}	
	}
	for(j = 1; ; ++j) { 
		if(!vis_v[j]) {
			vis_v[j] = 1;
			to_v[i] = j;
			return 1;
		}
	}
}
void change_c_to_ass() {
	int i, j;
	for(i = 1; i <= exp_num; ++i) {
		for(j = 0; j < 3; ++j) {
			int k = num[i][j][1];
			if(num[i][j][0] == 1)lst[k + var_num] = i;
			if(num[i][j][0] == 2)lst[k] = i;
		}
	}
	for(i = 1; i <= exp_num; ++i) {
		ll l1, l2, l3, r2, r3;
		l1 = num[i][0][1];
		l2 = num[i][1][1];
		l3 = num[i][2][1];
		r2 = num[i][1][0];
		r3 = num[i][2][0];
		if(r2 == 1) l2 += var_num;
		if(r3 == 1) l3 += var_num;
		int f1 = -1, f2 = -1, f3 = -1;
		if(to_t[l1]) f1 = 0;
		if(to_v[l1]) f1 = 1;
		if(to_t[l2]) f2 = 0;
		if(to_v[l2]) f2 = 1;
		if(r3) {
			if(to_t[l3]) f3 = 0;
			if(to_v[l3]) f3 = 1;
		}
		if(r3 && lst[l3] == i && op[i] != '-' && op[i] != '/') {
			swapll(&l2, &l3); swapll(&r2, &r3); swap(&f2, &f3);
			swapll(&num[i][1][0], &num[i][2][0]); 
			swapll(&num[i][1][1], &num[i][2][1]); 
		}
		if(r2 == 1 && !to_v[l2] && !to_t[l2]) {
			printf("t16 = a%d\n", l2 - var_num);
			f2 = add(l2);
			if(l2 == l3 && r2 == r3) f3 = f2;
			if(to_t[l2]) printf("t%d = t16\n", to_t[l2]);
			else printf("v%d = t16\n", to_v[l2]);
		}
		if(r3 == 1 && !to_v[l3] && !to_t[l3]) {
			printf("t16 = a%d\n", l3 - var_num);
			f3 = add(l3);
			if(to_t[l3])printf("t%d = t16\n", to_t[l3]);
			else printf("v%d = t16\n", to_v[l3]);
		}
		if(lst[l2] != i || l2 == l3) {
			if(f1 == -1) {
				f1 = add(l1);
			}
			if(!f1) {
				printf("t%d = ", to_t[l1]);
				if(!f2) {
					printf("t%d\n", to_t[l2]);
				} else {
					printf("v%d\n", to_v[l2]);
				}
			} else {
				if(!f2) {
					printf("v%d = t%d\n", to_v[l1], to_t[l2]);
				} else {
					printf("t16 = v%d\n", to_v[l2]);
					printf("v%d = t16\n", to_v[l1]);
				}
			}
		} else {
			if(!f2) {
				to_t[l1] = to_t[l2];
				to_t[l2] = 0;
			} else {
				to_v[l1] = to_v[l2];
				to_v[l2] = 0;
			}
			f1 = f2;
		}
		if(!f1) {
			printf("t%d %c= ", to_t[l1], op[i]);
			if(!r3) printf("%d\n", l3);
			else {
				if(!f3) {
					printf("t%d\n", to_t[l3]);
				} else printf("v%d\n", to_v[l3]);
			}
		} else {
			int flag = 0;
			if(!r3) {
				flag = 1;
				printf("t16 = %d\n", l3);
			}
			printf("v%d %c= ", to_v[l1], op[i]);
			if(flag) printf("t16\n");
			else if(!f3) {
				printf("t%d\n", to_t[l3]);
			} else printf("v%d\n", to_v[l3]);
		}
		clear(i);
	}
	return;
}
int main() {
	init();
	change_c_to_ass();
	return 0;
}
