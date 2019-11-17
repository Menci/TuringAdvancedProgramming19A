#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#define BITS 8
#define Size_line 60
#define Size_varible 6
#define Size_x 60
#define ll long long

char s[100010], Op[100010]; //op 表示第i行的操作符号

int Last[100010], preg[100010], parray[100010], Tp[100010], pos[100010], Array[100010], First[100010]; 
// Last[i]表示变量vi最后一次出现在哪一行
// preg[i]表示第i个寄存器有没有被用过
// parray[i]表示第i个内存地址有没有被用过
// Tp[i]表示vi被存进寄存器还是内存 若为1，则为寄存器，2则为内存
// pos[i]表示vi被存进第几个寄存器或者内存
// Array[i]表示第i个内存的位置是rsp + Array[i]

int Rsp, Max_array, Max_reg;

struct node {
	int Type; //是a1,a2,a3还是v1,v2....v60还是立即数
	ll val; // 是a几,v几,多少
}Input[70][3];

char Print_reg[20][20], Print_a[20][20];//寄存器

void Init() {
	Print_reg[1][0] = '%', Print_reg[1][1] = 'r', Print_reg[1][2] = '9';
	Print_reg[2][0] = '%', Print_reg[2][1] = 'r', Print_reg[2][2] = '1', Print_reg[2][3] = '0';
	Print_reg[3][0] = '%', Print_reg[3][1] = 'r', Print_reg[3][2] = '1', Print_reg[3][3] = '1';
	Print_reg[4][0] = '%', Print_reg[4][1] = 'r', Print_reg[4][2] = 'b', Print_reg[4][3] = 'x';
	Print_reg[5][0] = '%', Print_reg[5][1] = 'r', Print_reg[5][2] = 'b', Print_reg[5][3] = 'p';
	Print_reg[6][0] = '%', Print_reg[6][1] = 'r', Print_reg[6][2] = '1', Print_reg[6][3] = '2';
	Print_reg[7][0] = '%', Print_reg[7][1] = 'r', Print_reg[7][2] = '1', Print_reg[7][3] = '3';
	Print_reg[8][0] = '%', Print_reg[8][1] = 'r', Print_reg[8][2] = '1', Print_reg[8][3] = '4';
	Print_reg[9][0] = '%', Print_reg[9][1] = 'r', Print_reg[9][2] = '1', Print_reg[9][3] = '5';

	Print_a[1][0] = '%', Print_a[1][1] = 'r', Print_a[1][2] = 'd', Print_a[1][3] = 'i';
	Print_a[2][0] = '%', Print_a[2][1] = 'r', Print_a[2][2] = 's', Print_a[2][3] = 'i';
	Print_a[3][0] = '%', Print_a[3][1] = 'r', Print_a[3][2] = 'c', Print_a[3][3] = 'x';

	printf("movq %%rdx, %%rcx\n");
	printf("movq $0, %%r8\n");
	Max_array = -1;
}

void Scanf() {
	for (int i = 0; i < Size_line; ++i) {
		int Cnt = -1;
		for (int j = 0; j < 5; ++j) {
			// special
			scanf ("%s", s);
			if (s[0] == '=') continue;
			if (j == 3) {
				Op[i] = s[0];
				continue;
			}
			++ Cnt;
			//init
			struct node Temp;
			Temp.Type = 0, Temp.val = 0;
			//input
			int Len = strlen(s);
			if (Cnt == 2) -- Len;
			// learn the type
			if (s[0] == 'a') {
				Temp.Type = 1;
				Temp.val = s[1] - '0';
			}

			else if (s[0] == 'v') {
				Temp.Type = 2;
				int sum = 0;
				for (int k = 1; k < Len; ++k) sum = sum * 10 + s[k] - '0';

				Last[sum] = i;
				Temp.val = sum;
			}

			else {
				ll sum = 0; int st = 0;
				if(s[0] == '-') st = 1;
				for (int k = st; k < Len; ++k) sum = sum * 10 + s[k] - '0';
				Temp.Type = 3;
				if(s[0] == '-') sum = -sum;
				Temp.val = sum;
			}
			Input[i][Cnt] = Temp;
		}
	}
}

void add_rsp(int l,int r) {
	for (int i = l; i <= r; ++i) Array[i] += BITS;
	Rsp += BITS;
}

int Find_reg() {
	for (int i = 1; i <= Size_varible; ++i) if(preg[i] == 0) {
		preg[i] = 1; 
		if (i > 3 && i > Max_reg) {
			printf("pushq %s\n", Print_reg[i]);
		}

		if (Max_reg < i) Max_reg = i;
		return i;
	}
	return 0;
}

int Find_array() {
	for (int i = 0; ; ++i) {
		if(!parray[i]) {
			parray[i] = 1;
			if (i > Max_array) {
				printf ("subq $8, %%rsp\n");
				add_rsp (0, i);
				Max_array = i;
			}
			return i;
		}
	}
}

void Print(ll nowval, int nowtype) {
	if (nowtype == 1) printf("a%d;\n", nowval);
	else if (nowtype == 2) {
		if (Tp[nowval] == 1) {
			printf("r%d;\n", pos[nowval]);
		}
		else {
			printf("S[%d];\n", pos[nowval]);
		}
	}
	else printf("%lld;\n", nowval);
}

char for_print[6], yinhao = '"';
void Clear(int nowline) {
	for (int i = 1; i <= Size_x; ++i) {
		if (nowline >= Last[i]) {
			if (Tp[i]) {
				if (Tp[i] == 1) {
					preg[pos[i]] = 0;
					printf ("xorq %s, %%r8\n", Print_reg[pos[i]]);
				}

				else {
					parray[pos[i]] = 0;
					printf ("xorq %d(%%rsp), %%r8\n", Array[pos[i]]);
				}
				Tp[i] = 0;
			}
		}
	}
}

void Print_number5(struct node Temp) {
	ll nowval = Temp.val;
	if (Tp[nowval] == 1) printf("%s, ", Print_reg[pos[nowval]]);
	else printf ("%d(%%rsp), ", Array[pos[nowval]]);
}

void Print_number2(struct node Temp) {
	ll nowval = Temp.val;
	if (Tp[nowval] == 1) printf("%s\n", Print_reg[pos[nowval]]);
	else printf ("%d(%%rsp)\n", Array[pos[nowval]]);
}

void Print_number1(struct node Temp, int ifmemory) {
	int nowtype = Temp.Type;
	ll nowval = Temp.val;
	printf ("movq ");
	
	if (nowtype == 1) printf ("%s, ", Print_a[nowval]);
	else if (nowtype == 3) printf ("$%lld, ", nowval);		
	else if (nowtype == 2) {
		if (Tp[nowval] == 1) {
			printf ("%s, ", Print_reg[pos[nowval]]);
		}
		else {
			if (ifmemory) {
				printf ("%d(%%rsp), %%rdx\n", Array[pos[nowval]]);
				printf ("movq %%rdx, ");
			}
			else printf ("%d(%%rsp), ", Array[pos[nowval]]);
		}
	}
}

void Printi(int i) {
	if (Op[i] == '-') printf("subq ");
	else if (Op[i] == '+') printf("addq ");
	else if (Op[i] == '*') printf ("imulq ");
	else printf("idivq ");
}

void Print_number3(struct node Temp, int i, int ifmemory) {
	
	int nowtype = Temp.Type;
	ll nowval = Temp.val;

	if (nowtype == 1) { 
		Printi(i); 
		printf ("%s, ", Print_a[nowval]);
	}
	else if (nowtype == 3) { 

		printf ("movq $%lld, %%rdx\n", nowval);
		Printi(i);
		printf ("%%rdx, ");
	}
	else if (nowtype == 2) {
		if (Tp[nowval] == 1) {
			Printi(i);
			printf ("%s, ", Print_reg[pos[nowval]]);
		}
		else {
			if (ifmemory) {
				printf ("movq %d(%%rsp), %%rdx\n", Array[pos[nowval]]);
				Printi(i);
				printf ("%%rdx, ");
			}
			else { Printi(i); printf ("%d(%%rsp), ", Array[pos[nowval]]);}
		}
	}
	
	if (ifmemory) {
		printf ("%%rax\n");
		printf ("movq %%rax, ");
	}
}

void Print_number4(struct node Temp, int i) {
	
	int nowtype = Temp.Type;
	ll nowval = Temp.val;
	
	printf ("movq $0, %%rdx\n");
	printf ("cqto\n");

	if (nowtype == 1) { Printi(i); printf ("%s\n", Print_a[nowval]);}

	else if (nowtype == 3) {
		printf ("subq $8, %%rsp\n");
		printf ("movq $%lld, 8(%%rsp)\n", nowval);
		Printi(i); printf("8(%%rsp)\n");
		printf ("addq $8, %%rsp\n");
	}

	else if (nowtype == 2) {
		if (Tp[nowval] == 1) {
			Printi(i);
			printf ("%s\n", Print_reg[pos[nowval]]);
		}
		else {
			Printi(i); printf ("%d(%%rsp)\n", Array[pos[nowval]]);
		}
	}
}

void Endinit() {
	for (int i = Size_varible; i >= 4; --i) {
		if (i <= Max_reg) printf("popq %s\n", Print_reg[i]);
	}

	printf ("movq %%r8, %%rax\n");
	printf ("addq $%d, %%rsp\n", Rsp);
}

void Process() {
	for (int i = 0; i < Size_line; ++i) {

		ll nowval = Input[i][0].val;
		int ifmemory = 0;
		if (!Tp[nowval]) {
			int Location = Find_reg();
			if (Location) {
				Tp[nowval] = 1;
				pos[nowval] = Location;
			}

			else {
				Tp[nowval] = 2;
				pos[nowval] = Find_array();
			}
		}
		
		if (Tp[nowval] == 2) {
			ifmemory = 1;
		}

		Print_number1(Input[i][1], ifmemory);
		Print_number2(Input[i][0]);

		if (Op[i] == '+' || Op[i] == '-' || Op[i] == '*') {
			if (ifmemory) {
				printf ("movq "); 
				Print_number5(Input[i][0]);
				printf ("%%rax\n");
			}
			Print_number3(Input[i][2], i, ifmemory);
			Print_number2(Input[i][0]);
		}

		else {
			printf ("movq "); Print_number5(Input[i][0]);
			printf ("%%rax\n");
			Print_number4(Input[i][2], i);
			printf ("movq %%rax, "); Print_number2(Input[i][0]);
		}

		Clear(i);
		printf("\n");
	}

	if (Rsp) printf ("addq $%d, %%rsp\n", Rsp);
	for (int i = Size_varible; i >= 4; --i) {
		if (i <= Max_reg) printf("popq %s\n", Print_reg[i]);
	}

	printf ("movq %%r8, %%rax\n");
	printf ("ret\n");
}

int main() 
{
	//freopen ("bona.in", "r", stdin);
	//freopen ("bona.out", "w", stdout);

	Init();
	Scanf();
	Process();
	return 0;
}
