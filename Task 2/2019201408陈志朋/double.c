#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "definition.h"
#include "double_io.h"
#include "calculation.h"

#define ignore 0
#define work   1
#define clean  2

#define is_num 0
#define is_op  1
#define is_end 2
#define all_end 3

int n, last = -1, top_num = 0, top_op = 0, kind, sum = 0, N[505], bin[505], head, tail, len;
char op = ' ', sta_op[1000000];
Double Num, sta_num[1000000];
bool flag;
int Get_Mod() {
	int i, x = 0;
	for (i = head; i <= tail; ++i) {
		x += N[i];
		if (x >= 2) {
			N[i] = x / 2; x %= 2;
		}
		else N[i] = 0;
		x *= 10;
	}
	while (!N[head] && head < tail) head++;
	return x / 10;
}
void Dec_to_Bin_int() {
	if (!N[head])return;
	do {
		bin[len--] = Get_Mod();
	}while (N[head]);
	int i, j, k; len++;
	for (i = 500, j = len; i >= j; --i, ++j){
		k = bin[i]; bin[i] = bin[j]; bin[j] = k;
	}
}
int Mul_2() {
	int i, x = 0;
	for (i = tail; i >= head; --i) {
		N[i] *= 2; N[i] += x;
		x = N[i] / 10;
		N[i] %= 10;
	}
	return x;
}
void Dec_to_Bin_double() {
	int i;
	for (i = 1; i <= Max_Frac_Bin * 5; ++i) 
		bin[--len] = Mul_2();
}
bool Judge_Num(char c) {
	return (c >= '0' && c <='9');
}
bool Judge_Op(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')');
}
int Get(Double *val, char *op) {
	if (*op == '\n') return is_end;
	if (*op == EOF) return all_end;
	
	bool flag = 0;
	int i, j, exp = 0, Max = 500, f;
	unsigned long long tmp = 0;
	char c = getchar();
	while (!Judge_Num(c) && !Judge_Op(c)) {
		if (c == '\n') return is_end;
		if (c == EOF) return all_end;
		c = getchar();
	}
	if (Judge_Op(c)) {
		if (!(last == is_op && (*op) != '(' && (*op) != ')' && c =='-')) {
			*op = c; return is_op;
		}
	}
	head = 1; tail =0;
	for (; len <= 501; ++len) bin[len] = 0;
	len = 500;
	if (c == '-') f = -1;
	else {
		f = 1; N[++tail] = c - '0';
	}
	for (c = getchar(); Judge_Num(c); c = getchar())
		N[++tail] = c - '0';
	Dec_to_Bin_int();
	exp = 500 - len;
	
	if (c == '.') {
		head = 1; tail = 0;
		for (c = getchar(); Judge_Num(c); c = getchar()) {
			N[++tail] = c - '0';
		}
		Dec_to_Bin_double();
	}
	for (i = 500; bin[i] == 0 && i >= len; --i) exp--;
	if (exp < 0)Max += exp;
	Round(bin, &Max);
	exp += Bias;
	for (i = Max - 1, j = 0; j < Max_Frac_Bin; ++j, --i)
		tmp = (tmp << 1) | (bin[i]);
	tmp |= ((unsigned long long)exp) << 52;
	if (f == -1)tmp |= 1ll << 63;
	*val = Convert_to_MyDouble(tmp);
	*op = c;
	return is_num;
}
int Change(char op) {
	if (op == '+' || op == '-') return 1;
	else if (op == '*' || op == '/') return 2;
}
int check(char op1, char op2) {
	if (op1 == ')') return clean;
	else if (op1 == '(' || op2 == '(') return ignore;
	else if (Change(op1) > Change(op2)) return ignore;
	else return work;
}
void solve_work() {
	Double tmp;
	tmp = Convert_to_MyDouble(Calc(Convert_to_StdDouble(sta_num[top_num-1]), 
	                               Convert_to_StdDouble(sta_num[top_num]), 
								   sta_op[top_op]));
	top_num--; top_op--;
	sta_num[top_num] = tmp;
	sum--;
}
void solve_clean() {
	Double tmp;
	while (sta_op[top_op] != '(') solve_work();
	top_op--;
}
int main ()
{
	while (last != all_end){
		last = Get(&Num, &op);
		if (last == is_end || last == all_end) break;
		flag = 1;
		top_op = top_num = 0;
		sta_op[++top_op] = '(';
		while (last != is_end && last != all_end) {
			if (!flag) {
				if (last == is_num && Judge_Op(op)) {
					last = is_op;
				}
				else last = Get(&Num, &op);
			}
			else flag = 0;
			
			if (last == is_num) {
				sta_num[++top_num] = Num;
			}
			else if (last == is_op){
				if (op != '(' && op != ')') sum++;
				do {
					kind = check(op, sta_op[top_op]);
					if (kind == ignore) break;
					else if (kind == work) solve_work();
					else if (kind == clean) {
						solve_clean(); break;
					}
				} while (1);
				if (op != ')') sta_op[++top_op] = op;
			}
		}
		solve_clean();
		Printf(sta_num[top_num], 8); printf("\n");
		fflush(stdout);
	}
	return 0;
}
