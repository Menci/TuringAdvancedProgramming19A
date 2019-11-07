#include <ctype.h>
#include <stdio.h>
#include <string.h>
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
char ass_str[10][N];
int lst[N], to_v[N], to_t[N], vis_v[N], vis_t[N], maxv;
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
	strcpy(ass_str[1], "%rcx");//存数 
	strcpy(ass_str[2], "%r8");//存数
	strcpy(ass_str[3], "%r9");
	strcpy(ass_str[4], "%r10");
	strcpy(ass_str[5], "%r11");
	strcpy(ass_str[6], "%rdi");
	strcpy(ass_str[7], "%rsi");
	strcpy(ass_str[8], "%rdx");//除法有用 
	strcpy(ass_str[9], "%rax");
	to_t[1 + var_num] = 6;
	to_t[2 + var_num] = 7;
	to_t[3 + var_num] = 5;
	return;
}
void clear(int i) {
	int j;
	for(j = 0; j < 3; ++j) {
		int k = num[i][j][1];
		if(num[i][j][0] == 0) continue;
		if(num[i][j][0] == 1) k += var_num;
		if(lst[k] == i) {
			vis_t[to_t[k]] = 0; to_t[k] = 0;
		}
	}
	return;
}
int add(int i) {
	int j;
	for(j = 3; j <= 4; ++j) {
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
			if(j > maxv) maxv = j;
			return 1;
		}
	}
}
void v_print(int x) {
	int y = maxv - x;
	printf("%d(%%rsp)", y << 3);
	return;
}
void change_c_to_ass() {
	puts("movq $0, %rax");
	puts("movq %rdx, %r11");
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
		if(lst[l2] != i || l2 == l3) {
			if(f1 == -1) {
				f1 = add(l1);
			}
			if(!f1) {
				if(!f2) {
					printf("movq %s, ", ass_str[to_t[l2]]);
				} else {
					printf("movq "); v_print(to_v[l2]); printf(", ");
				}
				printf("%s\n", ass_str[to_t[l1]]);
			} else {
				if(!f2) {
					printf("pushq %s\n", ass_str[to_t[l2]]);
				} else {
					printf("pushq "); v_print(to_v[l2] + 1); printf("\n");
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
		if(op[i] == '+' || op[i] == '-') {
			if(r3 && f3) {
				printf("movq "); v_print(to_v[l3]); printf(", %%rcx\n");
			}
			if(op[i] == '+') printf("addq ");
			if(op[i] == '-') printf("subq ");
			if(!r3) printf("$%lld", l3);
			else {
				if(!f3) {
					printf("%s", ass_str[to_t[l3]]);
				} 
				else printf("%%rcx");
			}
			printf(", ");
			if(!f1) printf("%s\n", ass_str[to_t[l1]]);
			else v_print(to_v[l1]), puts("");
		}
		if(op[i] == '*') {
			if(r3 && f3) {
				printf("movq "); v_print(to_v[l3]); printf(", %%rcx\n");
			}
			if(f1) {
				printf("movq "); v_print(to_v[l1]); printf(", %%r8\n");
			}
			printf("imulq ");
			if(!r3) printf("$%lld", l3);
			else {
				if(!f3) {
					printf("%s", ass_str[to_t[l3]]);
				} else printf("%%rcx");
			}
			printf(", ");
			if(!f1) printf("%s\n", ass_str[to_t[l1]]);
			else puts("%r8");
			if(f1) {
				printf("movq %%r8, "); v_print(to_v[l1]); puts("");
			}
		}
		if(op[i] == '/') {
			puts("movq %rax, %r8");
			printf("movq ");
			if(!f1) printf("%s", ass_str[to_t[l1]]);
			else v_print(to_v[l1]);
			printf(", %%rax\n");
			puts("cqto");
			if(!r3) {
				printf("movq $%lld, %%rcx\n", l3);
				puts("idivq %rcx");
			} else {
				printf("idivq ");
				if(!f3) {
					printf("%s\n", ass_str[to_t[l3]]);
				} else v_print(to_v[l3]), puts("");
			}
			printf("movq %%rax, ");
			if(!f1) {
				printf("%s\n", ass_str[to_t[l1]]);
			} 
			else v_print(to_v[l1]), puts("");
			puts("movl $0, %eax");
			puts("movq %r8, %rax");
		}
		printf("xorq ");
		if(!f1) printf("%s", ass_str[to_t[l1]]);
		else v_print(to_v[l1]);
		printf(", %%rax\n");
		clear(i);
	}
	while(maxv--) puts("popq %rcx");
	puts("ret");
	return;
}
int main() {
	init();
	change_c_to_ass();
	return 0;
}
