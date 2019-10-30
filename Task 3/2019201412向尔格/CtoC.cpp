#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#define Size_line 60
#define Size_varible 15
#define Size_x 60
#define ll long long

char s[100010], Op[100010];

int Last[100010], preg[100010], parray[100010], Tp[100010], pos[100010];

struct node {
	int Type;
	ll val;
}Input[70][3];

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

int Find_reg() {
	for (int i = 1; i <= Size_varible; ++i) if(preg[i] == 0) {
		preg[i] = 1; 
		return i;
	}
	return 0;
}

int Find_array() {
	for (int i = 0; ; ++i) {
		if(!parray[i]) {
			parray[i] = 1;
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
					printf("printf(%cx%d : ", yinhao, i);
					printf("%s", for_print);
					printf("%c, r%d);\n", yinhao, pos[i]);
				}

				else {
					parray[pos[i]] = 0;
					printf("printf(%cx%d : ", yinhao, i);
					printf("%s", for_print);
					printf("%c, S[%d]);\n", yinhao, pos[i]);
				}
				Tp[i] = 0;
			}
		}
	}
}

void Process() {
	for (int i = 0; i < Size_line; ++i) {
		//output firstline
		ll nowval = Input[i][0].val;
		bool flag = 0;
		if (!Tp[nowval]) {
			int Location = Find_reg();
			if (Location) {
				Tp[nowval] = 1;
				pos[nowval] = Location;
				printf("r%d = ", Location);
			}

			else {
				Tp[nowval] = 2;
				pos[nowval] = Find_array();

				if (Input[i][1].Type == 2 && Tp[Input[i][1].val] == 2) {
					flag = 1;
					printf ("r16 = S[%d];\n", pos[Input[i][1].val]);
					printf ("S[%d] = r16;\n", pos[nowval]);
				}
				else printf("S[%d] = ", pos[nowval]);
			}
		}

		else if (Tp[nowval] == 1) {
			printf("r%d = ", pos[nowval]);
		}

		else {
			if (Input[i][1].Type == 2 && Tp[Input[i][1].val] == 2) {
				flag = 1;
				printf ("r16 = S[%d];\n", pos[Input[i][1].val]);
				printf ("S[%d] = r16;\n", pos[nowval]);
			}
			else printf("S[%d] = ", pos[nowval]);
		}
		
		int nowtype = Input[i][1].Type;
		nowval = Input[i][1].val;
		//special judge
		if(!flag) Print(nowval, nowtype);
		flag = 0;

		nowval = Input[i][0].val;
		if (Tp[nowval] == 1) {
			printf("r%d %c= ", pos[nowval], Op[i]);
		}

		else {
			if (Input[i][2].Type == 2 && Tp[Input[i][2].val] == 2) {
				flag = 1;
				printf ("r16 = S[%d];\n", pos[Input[i][2].val]);
				printf("S[%d] %c= r16;\n", pos[nowval], Op[i]);
			}
			else printf("S[%d] %c= ", pos[nowval], Op[i]);
		}

		nowtype = Input[i][2].Type;
		nowval = Input[i][2].val;
		if(!flag) Print(nowval, nowtype);

		Clear(i);
		printf("\n");
	}
}

int main() 
{
	//freopen ("bona.in", "r", stdin);
	//freopen ("test.c", "w", stdout);
	printf("#include <stdio.h>\n");
	printf("#include <stdlib.h>\n");
	printf("#include <string.h>\n");
	printf("#include <math.h>\n");
	printf("#include <stdbool.h>\n");
	printf("#include <stdint.h>\n");
	printf("\n");

	printf("int ");
	for (int i = 1; i <= Size_varible+1; ++i) printf("r%d, ", i);
	for (int i = 1; i <= 3; ++i) printf("a%d, ", i);
	printf("S[100010];\n");

	printf("int main()\n");
	printf("{\n");

	for_print[0] = '%';
	for_print[1] = 'd';
	for_print[2] = '\\';
	for_print[3] = 'n';
	Scanf();
	Process();
	printf("return 0;\n");
	printf("}\n");
	return 0;
}
