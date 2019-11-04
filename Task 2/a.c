#include <stdio.h>
double a;
double sta[10000000];
char ys[10000000];
char s[10000000];
int yxj[200];
int main()
{
	yxj['+'] = 1;
	yxj['-'] = 1;
	yxj['*'] = 2;
	yxj['/'] = 2;
	yxj['('] = 100;
	while(1){
		char ch;
		int len = 0, i, pla = -1, ned = 1, top_s = 0, top_y = 0, flag = 0, is_negative = 1;
		double sav = 1, number = 0, ans = 0;
		while((ch = getchar()) != '\n'){
			if(ch == ' ') continue;
			s[len ++] = ch;
			if(ch == EOF) return 0;
		}
		while(s[pla] != 0){
			if(ned && (s[pla] == '+' || s[pla] == '-' || (s[pla] <= '9' && s[pla] >= '0'))){
				while(1){
					if((s[pla] > '0'||s[pla] < '9') && s[pla] != '.'){
						sta[top_s ++] = -1 * number;
						is_negative = 1, number = 0, flag = 0, sav = 1;
						break;
					}
					if(s[pla] == '-') is_negative = -1;
					else if(s[pla] == '.') flag = 1;
					else{
						if(flag) number += sav * (s[pla] - '0');
						else number = number * 10 + s[pla] - '0';
					}
					if(flag) sav /= 10;
					pla ++;
				}
			}
			else{
				if(s[pla] == '('){
					ys[top_y ++] = '(';
					continue;
				}
				if(s[pla] == ')'){
					while(ys[top_y - 1] != '('){
						if(ys[top_y - 1] == '*') sta[top_s - 1] *= sta[top_s--];
						else if(ys[top_y - 1] == '/') sta[top_s - 1] /= sta[top_s--];
						else if(ys[top_y - 1] == '-') sta[top_s - 1] -= sta[top_s--];
						else if(ys[top_y - 1] == '+') sta[top_s - 1] += sta[top_s--];
					}
					continue;
				}
				while(top_y && yxj[s[pla]] >= yxj[ys[top_y - 1]]){
					if(ys[top_y - 1] == '*') sta[top_s - 1] *= sta[top_s--];
					else if(ys[top_y - 1] == '/') sta[top_s - 1] /= sta[top_s--];
					else if(ys[top_y - 1] == '-') sta[top_s - 1] -= sta[top_s--];
					else if(ys[top_y - 1] == '+') sta[top_s - 1] += sta[top_s--];
				}
				ys[top_y ++] = s[pla];
			}
		}
		while(top_y){
			if(ys[top_y - 1] == '*') sta[top_s - 1] *= sta[top_s--];
			else if(ys[top_y - 1] == '/') sta[top_s - 1] /= sta[top_s--];
			else if(ys[top_y - 1] == '-') sta[top_s - 1] -= sta[top_s--];
			else if(ys[top_y - 1] == '+') sta[top_s - 1] += sta[top_s--];
		}
		printf("%.10lf\n", sta[0]);
	}
	return 0;
}
