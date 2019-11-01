#include "dzy.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BITS 8
#define Bias 1023
#define ULL unsigned long long

char s[100010];

struct node Suf[100010], Stack[100010];

int Cnt_stack, Cnt_suf;

void In_stack(char op) {

	int Now_level = 2;
	if (op == '-' || op == '+') Now_level = 0;
	else if (op == '*' || op == '/') Now_level = 1;

	if (op != '(' && op != ')') {
		while (Cnt_stack) {
			if (Stack[Cnt_stack].Sig == '(') break;
			if (Now_level <= Stack[Cnt_stack].Level) { // = wait to assure
				Suf[++Cnt_suf] = Stack[Cnt_stack];
				-- Cnt_stack;
			}
			else break;
		}
	}
	
	else if (op == ')') {
		while (Cnt_stack) {
			if (Stack[Cnt_stack].Sig != '(') {
				Suf[++Cnt_suf] = Stack[Cnt_stack];
			}
			
			-- Cnt_stack;
			if (Stack[Cnt_stack + 1].Sig == '(') break;
		}
	}

	if(op != ')') {
		++ Cnt_stack;
		Stack[Cnt_stack].Sig = op;
		Stack[Cnt_stack].Level = Now_level;
	}
}

int Int[100010], Temp[100010], High[100010], Shit[100010]; 
// Int[] : the part of integer, it also save all the number
// Temp[] : temporary array
// High[] : high accuracy array

struct Test { // Test scanf
	char Test_db[8];
};

void Scanf_test(struct node T) {
	struct Test Test_scanf;
	for (int j = 0; j < BITS; ++j) Test_scanf.Test_db[j] = T.db[j];
	printf("%.50le\n", *(double*)&Test_scanf);
}

void Handle() {
	for (int i = 1; i <= Cnt_suf; ++i) {
		if(Suf[i].Sig == 'n') {
			Stack[++ Cnt_stack] = Suf[i];
		}

		else {
			Stack[Cnt_stack - 1] = bona(Stack[Cnt_stack-1], Stack[Cnt_stack], Suf[i].Sig);
			Stack[Cnt_stack - 1].Sig = 'n';
			Stack[Cnt_stack - 1].Level = 3;
			-- Cnt_stack;
		}
	}
}

int Print[1010];

void Done_printf(struct node T) {
	for (int i = 1; i <= 100; ++i) Int[i] = 0;
	int T_exp = (int) Exp(T);

	if (T_exp == 0) { printf("0\n"); return; }
	if(get_sign(T)) printf("-");
	if(INF(T)) { printf("INF\n"); return;}
	if(Is_nan(T)) {printf("nan\n"); return;}

	T_exp = T_exp - Bias;
	ULL Frac = Val(T);

	int Cnt = 0;
	Int[++ Cnt] = 1;

	for (int i = 0; i < 52; ++i) {
		if (Frac & (1LL << 51 - i)) Int[++ Cnt] = 1;
		else Int[++ Cnt] = 0;
	}

	ULL Integer = 0;
	if (T_exp >= 0) {
		if(T_exp + 1 <= Cnt) {
			for (int i = T_exp + 1; i ; --i) {
				if (Int[i]) Integer += (1LL << (T_exp + 1 - i) );
			}
			printf("%llu", Integer);
		}

		else {
			for (int i = Cnt; i; --i) {
				if (Int[i]) Integer += (1LL << (Cnt - i) );
			}
			//High accuracy
			int Dec = 0;
			while (Integer) {
				Temp[++ Dec] = (Integer % 10);
				Integer /= 10;
			}

			for (int i = 1; i <= Dec; ++i) High[i] = Temp[i];
			int sum = T_exp + 1 - Cnt;

			while (sum) {
				-- sum;
				for (int i = 1; i <= Dec + 1; ++i) {
					High[i] <<= 1;
					if (High[i-1] >= 10) {
						++ High[i];
						High[i-1] -= 10;
					}
				}
				if (High[Dec+1]) ++ Dec;
			}

			if (!High[Dec]) -- Dec;
			for (int i = Dec; i >= 1; --i) printf("%d", High[i]);
			for (int i = Dec; i >= 1; --i) High[i] = 0;
		}
	}
	else printf("0");

	printf(".");
	// Decimal
	__int128 Value = 0; 
	ULL Accumulate = 0; 

	int Up = 60;
	for (int i = T_exp + 2; i <= T_exp + Up; ++i) {
		if (i > 0) {
			if (Int[i]) {
				Value += (1LL << (T_exp + Up - i) );
			}
		}
	}

	ULL Save = Value;
	Accumulate = (1LL << Up-1);

	ULL Stable = 1e18;
	Value = Value * Stable;
	Value = Value / Accumulate;
	ULL COMP = (ULL)Value;

	Cnt = 0;
	while (Value > 0) {
		Temp[++ Cnt] = Value % 10;
		Value /= 10;
	}

	int Calc_cnt = 0;
	while (Save > 0) {
		++ Calc_cnt;
		Save /= 10;
	}
	
	for (int i = 18; i > Cnt; --i) printf("0");

	for (int i = Cnt; i; --i) printf("%d", Temp[i]);
	printf("\n");
}


void Done_scanf() {
	char op;
	int Len = 0, Tina = 0;
	//scanf
	while (1) {
		Len = 0;
		while(1) {
			op = getchar();
			if(op == '\n') break;
			if(op == EOF) {
				Tina = 1;
				break;
			}
			s[Len++] = op;
		}

		if(Tina) break;

		char Lastop = '#';

		for (int i = 0; i < Len; ++i) {
			if (s[i] == ' ') continue;
			if (s[i] == '+' || s[i] == '*' || s[i] == '/' || s[i] == '(' || s[i] == ')') In_stack(s[i]);
			else if (s[i] == '-' && (i && Lastop >= '0' && Lastop <= '9') ) In_stack(s[i]);
			else {
				ULL Integer = 0; int Dot = 0, If_minus = 0;// Dot : position of '.'
				if (s[i] == '-') ++i, If_minus = 1;

				int Cnt = 0;
				int gross = 0;

				while (s[i] >= '0' && s[i] <= '9') {
					Temp[++gross] = s[i] - '0';
					++ i;
				}

				for (int i = 1; i <= gross; ++i) Shit[i] = Temp[gross - i + 1];

				while (gross) {
					int Last = 0;
					for (int i = gross; i; --i) {
						int nowval = Shit[i] + Last * 10;
						Shit[i] = nowval / 2;

						if (nowval % 2 == 0) Last = 0;
						else Last = 1;
					}

					if(!Shit[gross]) --gross;
					Temp[++ Cnt] = Last;
				}
				for (int j = Cnt; j >= 1; --j) {
					Int[j] = Temp[Cnt - j + 1];
				}

				Dot = Cnt;
				if (s[i] == '.') { // have decimal parts
					++i; int Dec = 0;
					while (s[i] >= '0' && s[i] <= '9') {
						Temp[++ Dec] = s[i] - '0';
						++ i;
					}

					for (int j = 1; j <= Dec; ++j) {
						High[j] = Temp[Dec - j + 1];
					}

					int Length = 300, sum = 0;
					while (sum < Length) {
						for (int j = 1; j <= Dec + 1; ++j) {
							High[j] <<= 1;
							if (High[j-1] >= 10) {
								High[j-1] -= 10;
								++ High[j];
							}
						}

						if (High[Dec+1]) High[Dec+1] = 0, Int[++ Cnt] = 1;
						else Int[++ Cnt] = 0;
						++ sum;
					}

					for (int j = 1; j <= Dec; ++j) High[j] = 0;
				}

				int Expect = 0; ULL Frac = 0;
				for (int j = 1; j <= Cnt; ++j) {
					if (Int[j] == 0) continue;
					//get exp
					Expect = Dot - j + Bias;
					//get frac
					for (int k = j + 52; k >= j + 1; --k) {
						if (Int[k]) Frac += (1LL << (j + 52 - k) );
					}
					// ROUND
					if (Int[j+53]) {
						bool flag = 0;
						for (int k = j + 54; k <= Cnt; ++k) {
							if (Int[k]) {
								flag = 1;
								break;
							}
						}
						//round to even
						if (flag) ++ Frac;
						else if (Int[j+52]) ++ Frac;
					}
					break;
				}

				ULL Value = Frac;
				for (int j = 0; j < 11; ++j) {
					if (Expect & (1 << j) ) Value += (1LL << (j + 52) );
				}

				if(If_minus) Value += (1LL << 63);
				// transform
				ULL Accumulate = 0;

				++ Cnt_suf;
				for(int j = 0;j < BITS; ++j) Suf[Cnt_suf].db[j] = 0; // Init

				for (int j = 0; j < BITS; ++j) {
					for (int k = 0; k < BITS; ++k) {
						if ( (Value >> (k + Accumulate))&1) Suf[Cnt_suf].db[j] += (1 << k);
					} Accumulate += BITS;
				}	
				
				Suf[Cnt_suf].Sig = 'n'; //num
				Suf[Cnt_suf].Level = 3;
				
				for (int j = 1; j <= Cnt; ++j) Int[j] = 0; //clear
				-- i;
			}
			if(s[i] != ')' && s[i] != '(') Lastop = s[i];
		}

		while(Cnt_stack) {
			Suf[++ Cnt_suf] = Stack[Cnt_stack];
			-- Cnt_stack;
		}

		Handle();
		Done_printf(Stack[1]);

		for (int i = 0; i <= Len + 10; ++i) s[i] = '@';
		Cnt_stack = Cnt_suf = 0;
		fflush(stdout);
	}
}

int main() {

	Done_scanf();

	return 0;
}
