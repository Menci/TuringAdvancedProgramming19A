#include <stdio.h>
#include <string.h>
#define MAX_L 60
#define MAX_REG 6
#define STACK_SPACE 64
char oper[MAX_L + 1], t1[4], t2[4], t3[4], t4[32];

int max_reg, max_mem = 0, forb = -1;
int sreg[MAX_REG], loc[MAX_L + 5], sram[MAX_L * 2], num1[MAX_L + 5], num2[MAX_L + 5], st[MAX_L + 5], typ[MAX_L + 5];
int las[MAX_L + 5];
char regName[MAX_REG + 1][6] = {"%rdi", "%rsi", "%rbx", "%r8", "%r9", "%r10", "%rax"};

void getMemAddr(char *s, int x) {
	int num = (STACK_SPACE / 8 - 1 - x) * 8;
	if(num) sprintf(s, "%d(%%rsp)", num);
	else sprintf(s, "(%%rsp)");
}

void printExpr(int lv, char op, int rv, char ltyp, char rtyp) {
	char strl[16], strr[16];
	if(ltyp == 'r') sprintf(strl, "%s", regName[lv]);
	else if(ltyp == 'p') getMemAddr(strl, lv);
	if(rtyp == 'r') sprintf(strr, "%s", regName[rv]);
	else if(rtyp == 'p') getMemAddr(strr, rv);
	else sprintf(strr, "$%d", rv);
	if(op == ' ') printf("movq %s, %s\n", strr, strl);
	else if(op == '+') printf("addq %s, %s\n", strr, strl);
	else if(op == '-') printf("subq %s, %s\n", strr, strl);
	else if(op == '*') printf("imulq %s, %s\n", strr, strl);
	else {
		if(lv != MAX_REG) printf("movq %s, %%rax\n", strl);
		printf("cqto\n");
		if(rtyp != '$') printf("idivq %s\n", strr);
		else {
			printf("movq $%d, %%rcx\n", rv);
			printf("idivq %%rcx\n");
		}
		if(lv != MAX_REG) printf("movq %%rax, %s\n", strl);
	}
}
void xorResult(int x){
	printf("xorq %s, %%r11\n", regName[x]);
}

void printFileHead() {
	printf("subq $%d, %%rsp\n", STACK_SPACE);
	printf("movq $0, %%r11\n");
}

void printFileEnd() {
	printf("addq $%d, %%rsp\n", STACK_SPACE);
	printf("movq %%r11, %%rax\n");
	printf("ret\n");
}
int getMem() {
	for(int i = 0; i < max_mem; ++i) 
		if(sram[i] == -1) return i;
	return max_mem++;
}
int getReg() {
	int ret = -1;
	for(int i = 0; i < MAX_REG; ++i) if(i != forb) {
		if(sreg[i] == 0) return i;
		if(sreg[i] == -1) ret = i;
	}
	if(ret != -1) {
		++max_reg;
		return ret;
	}
	int curmx = -1;
	for(int i = 0; i < MAX_REG; ++i) if((i != forb) && las[sreg[i]] > curmx) {
		curmx = las[sreg[i]];
		ret = i;
	}
	int newp = getMem();
	printExpr(newp, ' ', ret, 'p', 'r');
	sram[newp] = sreg[ret];
	st[sreg[ret]] = 2, loc[sreg[ret]] = newp;
	return ret;
}
void recycleSpace(int t) {
	for(int i = 0; i < MAX_REG; ++i) if(sreg[i] >= 0) {
		if(las[sreg[i]] <= t) {
			st[sreg[i]] = loc[sreg[i]] = -1;
			sreg[i] = 0;
		}
	}
	for(int i = 0; i < max_mem; ++i) if(sram[i] >= 0) {
		if(las[sram[i]] <= t) {
			st[sram[i]] = loc[sram[i]] = -1;
			sram[i] = 0;
		}
	}
}
int main() {
	int vn, s1, s2, fl;
	char p;
	for(int i = 1; i <= MAX_L; ++i) las[i] = i;
	for(int i = 1; i <= MAX_L; ++i) {
		scanf("%s %*s %s %s %s", t1, t2, t3, t4);
		if(t2[0] == 'a') s1 = atoi(t2 + 1) + MAX_L - 1, las[s1] = i;
		else s1 = atoi(t2 + 1), las[s1] = i;
		if(t4[0] == 'a') s2 = atoi(t4 + 1) + MAX_L - 1, fl = 1, las[s2] = i;
		else if(t4[0] == 'v') s2 = atoi(t4 + 1), fl = 1, las[s2] = i;
		else s2 = atoi(t4), fl = 2;
		typ[i] = fl, num1[i] = s1, num2[i] = s2, oper[i] = t3[0];
	}
	max_reg = 0;
	memset(st, -1, sizeof(st));
	memset(loc, -1, sizeof(loc));
	memset(sreg, -1, sizeof(sreg));
	memset(sram, -1, sizeof(sram));
	printFileHead();
	for(int i = 0; i < 3; ++i) {
		int cp = MAX_L + i;
		sreg[i] = cp;
		st[cp] = 1, loc[cp] = i;
	}
	printf("movq %%rdx, %%rbx\n");
	for(int i = 1; i <= MAX_L; ++i) {
		if(typ[i] == 1) {
			int p1 = num1[i], p2 = num2[i];
			if(st[p1] == 1) {
				if(las[p1] == i) {
					printExpr(loc[p1], oper[i], loc[p2], 'r', st[p2] == 1 ? 'r' : 'p');
					xorResult(loc[p1]);
					sreg[loc[p1]] = i, loc[i] = loc[p1], st[i] = 1;
					st[p1] = loc[p1] = -1;
				}
				else {
					forb = loc[p1];
					if(oper[i] == '/') {
						int newr = getReg(), oldr = loc[p1];
						printExpr(MAX_REG, ' ', oldr, 'r', 'r');
						printExpr(MAX_REG, '/', loc[p2], 'r',  st[p2] == 1 ? 'r' : 'p');
						xorResult(MAX_REG);
						printExpr(newr, ' ', MAX_REG, 'r', 'r');
						sreg[newr] = i, loc[i] = newr, st[i] = 1;
					}
					else {
						int newr = getReg(), oldr = loc[p1];
						printExpr(newr, ' ', oldr, 'r', 'r');
						sreg[newr] = p1, loc[p1] = newr;
						printExpr(oldr, oper[i], loc[p2], 'r', st[p2] == 1 ? 'r' : 'p');
						xorResult(oldr);
						sreg[oldr] = i, loc[i] = oldr, st[i] = 1;
					}
					
				}
			}
			else {
				forb = -1;
				if(oper[i] == '/') {
					printExpr(MAX_REG, ' ', loc[p1], 'r', 'p');
					printExpr(MAX_REG, '/', loc[p2], 'r',  st[p2] == 1 ? 'r' : 'p');
					xorResult(MAX_REG);
					int newr = getReg();
					printExpr(newr, ' ', MAX_REG, 'r', 'r');
					sreg[newr] = i, loc[i] = newr, st[i] = 1;
				}
				else {
					int newr = getReg(), oldp = loc[p1];
					printExpr(newr, ' ', oldp, 'r', 'p');
					printExpr(newr, oper[i], loc[p2], 'r', st[p2] == 1 ? 'r' : 'p');
					xorResult(newr);
					sreg[newr] = i, loc[i] = newr, st[i] = 1;
				}
			}
		}
		else {
			int p1 = num1[i];
			if(st[p1] == 1) {
				forb = loc[p1];
				int newr = getReg(), oldr = loc[p1];
				printExpr(newr, ' ', oldr, 'r', 'r');
				sreg[newr] = p1, loc[p1] = newr;
				printExpr(oldr, oper[i], num2[i], 'r', '$');
				xorResult(oldr);
				sreg[oldr] = i, loc[i] = oldr, st[i] = 1;
			}
			else {
				forb = -1;
				int newr = getReg(), oldp = loc[p1];
				printExpr(newr, ' ', oldp, 'r', 'p');
				printExpr(newr, oper[i], num2[i], 'r', '$');
				xorResult(newr);
				sreg[newr] = i, loc[i] = newr, st[i] = 1;
			}
		}
		recycleSpace(i);
	}
	printFileEnd();
	// fprintf(stderr, "max_reg = %d, max_mem = %d\n", max_reg, max_mem);
}