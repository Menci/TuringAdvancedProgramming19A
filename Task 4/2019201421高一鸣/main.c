#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 100010
#define pos(cnt,x) pos([flag[x][cnt]])
#define Reg_Size 10
#define Max 60

int getline_(char s[],int lim){
	int c,i;
	i=0;
	while((c=getchar())!=EOF&&c!='\n'&&i<lim-1)
		s[i++]=c;
	s[i]='\0';
	return i;
}
int inputNumber(char *c) {
	static bool iosig;
	int x=0;
	for (iosig = false;!isdigit(*c);c++) {
		if (*c == -1) return 0;
		if (*c == '-') iosig = true;	
	}
	for (x = 0; isdigit(*c);(c)++) {
		x*=10,x+=(*c)^'0';
	}
	if (iosig) x = -x;
	return x;
}
char s[100];
long long son[2][N];
int flag[2][N],last[4][N],cntt[3][100][100];
int *pos[4][N];
char opt[N],out[4]={'0','a','v'};
int jc[17],mem[N],*ms,**pjc[17];
int cntjc[17][100];
char reg[20][5]={"","%rdi","%rsi","%rbx","%rcx","%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15"};
int isReg(int *x) {
	
	if((x-jc)<=Reg_Size&&(x-jc)>=0) return 1;
	return 0;
}
void modify(int **s) {
	*s=ms;
}
char posChar[3][20];
int main() {
	puts("pushq\t%rbx"),puts("pushq\t%rsp"),puts("pushq\t%rbp"),puts("pushq\t%r12");
	puts("pushq\t%r13"),puts("pushq\t%r14"),puts("pushq\t%r15");
	puts("movq\t%rsp, %rbp");
	puts("xorq\t%r15,\t%r15");
	puts("movq\t%rdx,%rbx");
	int cnt=0;
	while(~scanf("%s",s)) {
		cnt++;
		scanf("%s",s);
		scanf("%s",s);
		last[2][cnt]=cnt;
		if(s[0]=='a') flag[0][cnt]=1;
		else if(s[0]=='v') flag[0][cnt]=2;
		else flag[0][cnt]=3;
		if(s[0]=='a'||s[0]=='v') {
			son[0][cnt]=inputNumber(s+1);
		} else son[0][cnt]=inputNumber(s);
		if(flag[1][cnt]!=3) last[flag[0][cnt]][son[0][cnt]]=cnt;
		for(int j=cnt;j<=Max;j++) if(flag[1][cnt]!=3) cntt[flag[0][cnt]][son[0][cnt]][j]++;
		scanf("%s",opt+cnt);
		scanf("%s",s);
		if(s[0]=='a') flag[1][cnt]=1;
		else if(s[0]=='v') flag[1][cnt]=2;
		else flag[1][cnt]=3;
		if(s[0]=='a'||s[0]=='v') {
			son[1][cnt]=inputNumber(s+1);
		} else son[1][cnt]=inputNumber(s);
		if(flag[1][cnt]!=3) last[flag[1][cnt]][son[1][cnt]]=cnt;
		for(int j=cnt;j<=Max;j++) if(flag[1][cnt]!=3) cntt[flag[1][cnt]][son[1][cnt]][j]++;
	}
	memset(jc,0,sizeof(jc));
	ms=mem+0;
	for(int i=1;i<=3;i++) {
		if(last[1][i]) {
			pos[1][i]=&jc[i],jc[i]=last[1][i],pjc[i]=&pos[1][i];
			memcpy(cntjc[i],cntt[1][i],sizeof(int)*Max);
		}
	}
	for(int i=1;i<=cnt;i++) {
		for(int j=1;j<=Reg_Size;j++) {
			if(jc[j]<i) jc[j]=0;
		}
		int ff=1;
		for(int j=1;j<=Reg_Size;j++) {
			if(jc[j]==0) ff=0;
		}
		if(ff) {
			int tmp=1,tmpp=9999999;
			for(int j=1;j<=Reg_Size;j++) {
				if(cntjc[j][Max]-cntjc[j][i]<tmpp) {
					tmp=j,tmpp=cntjc[j][Max]-cntjc[j][i];
				}
			}
			modify(&(*pjc[tmp]));
			printf("movq    %s,-%ld(%%rbp)\n",reg[tmp],(ms-mem)*sizeof(long long));
			ms++;
			jc[tmp]=0;
		}
		for(int j=1;j<=Reg_Size;j++) {
			if(jc[j]==0) {
				jc[j]=last[2][i];
				pos[2][i]=&jc[j];
				pjc[j]=&pos[2][i];
				memcpy(cntjc[j],cntt[2][i],sizeof(int)*Max);
				sprintf(posChar[2],"%s",reg[j]);
				if(flag[0][i]==3) {
					posChar[0][0]='$';
					sprintf(posChar[0]+1,"%lld",son[0][i]);
				} else {
					int *pos1=pos[flag[0][i]][son[0][i]];
					if(isReg(pos1)) {
						sprintf(posChar[0],"%s",reg[pos1-jc]);
					}
					else {
						sprintf(posChar[0],"-%ld(%%rbp)",(pos1-mem)*sizeof(long long));
					}
				}
				if(flag[1][i]==3) {
					posChar[1][0]='$';
					sprintf(posChar[1]+1,"%lld",son[1][i]);
				} else {
					int *pos2=pos[flag[1][i]][son[1][i]];
					if(isReg(pos2)) {
						sprintf(posChar[1],"%s",reg[pos2-jc]);
					}
					else {
						sprintf(posChar[1],"-%ld(%%rbp)",(pos2-mem)*sizeof(long long));
					}
				}
				printf("movq\t%s,\t%s\n",posChar[0],posChar[2]);
				if(opt[i]!='/') {
					if(opt[i]=='+') printf("addq\t%s,\t%s\n",posChar[1],posChar[2]);
					if(opt[i]=='-') printf("subq\t%s,\t%s\n",posChar[1],posChar[2]);
					if(opt[i]=='*') printf("imulq\t%s,\t%s\n",posChar[1],posChar[2]);
				} else {
					printf("movq\t%s,\t%%r14\n",posChar[1]);
					printf("movq\t%s,\t%%rax\n",posChar[2]);
					puts("cqto");
					printf("idivq\t%%r14\n");
					printf("movq\t%%rax,\t%s\n",posChar[2]);
				}
				
				j=17;
			}
		}
		printf("xorq\t%s,\t%%r15\n",posChar[2]);
	}
	puts("movq\t%r15,\t%rax");
	puts("popq\t%r15");
	puts("popq\t%r14"),puts("popq\t%r13"),puts("popq\t%r12");
	puts("popq\t%rbp"),puts("popq\t%rsp"),puts("popq\t%rbx");
	puts("ret");
}
