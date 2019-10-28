#include <stdio.h>
#include <string.h>
#define LOCAL_DEBUG
#define MAX_L 60
#define INPUT_FILE_NAME "1.txt"
#define MAX_REG 16
char oper[MAX_L + 1], t1[4], t2[4], t3[4], t4[32];

int max_reg, forb;
int sreg[MAX_REG], loc[MAX_L + 5], sram[MAX_L * 2], num1[MAX_L + 5], num2[MAX_L + 5], st[MAX_L + 5], typ[MAX_L + 5];
int las[MAX_L + 5];

void printExpr(int lv, char op, int rv, char ltyp, char rtyp) { // Indent with 4 spaces.
	printf("    %c%d %c= %c%d;\n", ltyp, lv, op, rtyp, rv);
}

void printFileHead() {
	puts("#include <stdio.h>");
	puts("int a1 = 998, a2 = 244, a3 = 353;");
	// printf("int ");
	// for(int i = 1; i <= 3; ++i) printf( (i == 3) ? " a%d;\n" : " a%d,", i);
	printf("int ");
	for(int i = 0; i < MAX_REG; ++i) printf( (i == (MAX_REG - 1)) ? " r%d;\n" : " r%d,", i);
	printf("int ");
	for(int i = 0; i < MAX_L; ++i) printf( (i == (MAX_L - 1)) ? " p%d;\n" : " p%d,", i);
	printf("int main() { \n");
	printf("    freopen(\"result1.txt\", \"w\", stdout);\n");
}

void printFileEnd() {
	printf("    return 0;\n}\n");
}
int max_mem = 0;
int getMem() {
	for(int i = 0; i < max_mem; ++i) 
		if(sram[i] == 0) return i;
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
	st[sreg[ret]] = 2, loc[sreg[ret]] = newp;
	sram[sreg[ret]] = loc[sreg[ret]];
	return ret;
}
void recycleSpace(int t) {
	for(int i = 0; i < MAX_REG; ++i) if(sreg[i] > 0) {
		if(las[sreg[i]] <= t) {
			#ifdef LOCAL_DEBUG
			printf("    /*RECYCLE (r%d,%d)*/\n", i, sreg[i]);
			#endif
			st[sreg[i]] = loc[sreg[i]] = -1;
			sreg[i] = 0;
		}
	}
	for(int i = 0; i < max_mem; ++i) if(sram[i] > 0) {
		if(las[sram[i]] <= t) {
			st[sram[i]] = loc[sram[i]] = -1;
			sram[i] = 0;
		}
	}
}
int main(int argc, char *argv[]) {
	freopen("output.c", "w", stdout);
	if(argc == 1) freopen("../testdata/" INPUT_FILE_NAME, "r", stdin);
	else {
		char filename[256];
		strcat(filename, "../testdata/");
		strcat(filename, argv[1]);
		freopen(filename, "r", stdin);
	}
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
	for(int i = 1; i <= 3; ++i) {
		int pos = getReg(), cp = MAX_L + i - 1;
		sreg[pos] = cp;
		st[cp] = 1, loc[cp] = pos;
		printExpr(pos, ' ', i, 'r', 'a');
	}

	for(int i = 1; i <= MAX_L; ++i) {
		if(typ[i] == 1) {
			int p1 = num1[i], p2 = num2[i];
			if(st[p1] == 1) {
				if(las[p1] == i) {
					printExpr(loc[p1], oper[i], loc[p2], 'r', st[p2] == 1 ? 'r' : 'p');
					sreg[loc[p1]] = i, loc[i] = loc[p1], st[i] = 1;
					st[p1] = loc[p1] = -1;
				}
				else {
					forb = loc[p1];
					int newr = getReg(), oldr = loc[p1];
					printExpr(newr, ' ', oldr, 'r', 'r');
					sreg[newr] = p1, loc[p1] = newr;
					printExpr(oldr, oper[i], loc[p2], 'r', st[p2] == 1 ? 'r' : 'p');
					sreg[oldr] = i, loc[i] = oldr, st[i] = 1;
				}
			}
			else {
				forb = -1;
				int newr = getReg(), oldp = loc[p1];
				printExpr(newr, ' ', oldp, 'r', 'p');
				printExpr(newr, oper[i], loc[p2], 'r', st[p2] == 1 ? 'r' : 'p');
				sreg[newr] = i, loc[i] = newr, st[i] = 1;
			}
		}
		else {
			int p1 = num1[i];
			if(st[p1] == 1) {
				forb = loc[p1];
				int newr = getReg(), oldr = loc[p1];
				printExpr(newr, ' ', oldr, 'r', 'r');
				sreg[newr] = p1, loc[p1] = newr;
				printExpr(oldr, oper[i], num2[i], 'r', ' ');
				sreg[oldr] = i, loc[i] = oldr, st[i] = 1;
			}
			else {
				forb = -1;
				int newr = getReg(), oldp = loc[p1];
				printExpr(newr, ' ', oldp, 'r', 'p');
				printExpr(newr, oper[i], num2[i], 'r', ' ');
				sreg[newr] = i, loc[i] = newr, st[i] = 1;
			}
		}
		
		#ifdef LOCAL_DEBUG
		// fprintf(stderr, "time = %d,mem = %d, reg = %d\n", i, max_mem, max_reg);
		int x = -1;
		for(int j = 0; j < MAX_REG; ++j) if(sreg[j] == i) x = j;
		printf("    printf(\"%%d\\n\", r%d);\n", x);
		printf("    /* \n");
		printf("    ");
		for(int j = 0; j < MAX_REG; ++j) printf("%3d", sreg[j]); puts("");
		printf("    ");
		for(int j = 0; j < MAX_REG; ++j) printf("%3d", j); puts("");
		puts("");
		printf("    */\n");
		#endif
		recycleSpace(i);
	}
	printFileEnd();
	fprintf(stderr, "max_reg = %d, max_mem = %d\n", max_reg, max_mem);
}