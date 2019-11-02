#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 100010
#define pos(cnt,x) pos([flag[x][cnt]])
#define Reg_Size 11
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
//,rbx,rcx,rdx,rsp,rbp,rsi,rdi,r8,r9,r10,r11,r12,r13,r14,r15
int isReg(int *x) {
	if((x-jc)/sizeof(int)<=Reg_Size&&(x-jc)>=0) return 1;
	return 0;
}
void modify(int **s) {
	s=&ms;
}
char posChar[3][20];
int main() {
//	freopen("../testdata/10.txt","r",stdin);
	puts("pushq    %rbx"),puts("pushq    %rsp"),puts("pushq    %rbp"),puts("pushq    %r12");
	puts("pushq    %r13"),puts("pushq    %r14"),puts("pushq    %r15");
	puts("movq    %rsp, %rbp"),puts("addq    $20000032,%rbp");
	puts("movq %rdx,%rbx");
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
			printf("movq    -%ld(%%rbp),%s\n",(ms-mem)*sizeof(long long),reg[tmp]);
//			printf("mem[%ld] = reg%d;\n",ms-mem,tmp);
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
//				printf("reg%d = ",j);
				if(flag[0][i]==3) {
					posChar[0][0]='$';
					sprintf(posChar[0]+1,"%lld",son[0][i]);
//					printf("$%d\n",son[0][i]);
//					printf("%d;\n",son[0][i]);
				} else {
//					printf("%d %d\n",flag[0][i],son[0][i]);
					int *pos1=pos[flag[0][i]][son[0][i]];
					if(isReg(pos1)) {
						sprintf(posChar[0],"%s",reg[pos1-jc]);
//						printf("%s\n",reg[pos1-jc]);
					}
					else {
						sprintf(posChar[0],"-%ld(%%rbp)",pos1-mem);
//						printf(" -%ld(%%rbp)\n",pos1-mem);
					}
//					if(isReg(pos1)) printf("reg%ld;\n",pos1-jc);
//					else printf("mem[%ld];\n",pos1-mem);
				}
//				if(opt[i]=='+') printf("addq    %s, ")
//				printf("reg%d %c= ",j,opt[i]);
				if(flag[1][i]==3) {
					posChar[1][0]='$';
					sprintf(posChar[1]+1,"%lld",son[1][i]);
//					printf("%d;\n",son[1][i]);
				} else {
					int *pos2=pos[flag[1][i]][son[1][i]];
					if(isReg(pos2)) {
						sprintf(posChar[1],"%s",reg[pos2-jc]);
//						printf("reg%ld;\n",pos2-jc);
					}
					else {
						sprintf(posChar[1],"-%ld(%%rbp)",pos2-mem);
//						printf("mem[%ld];\n",pos2-mem);
					}
				}
				printf("movq %s, %s\n",posChar[0],posChar[2]);
				if(opt[i]!='\\') {
					if(opt[i]=='+') printf("addq %s, %s\n",posChar[1],posChar[2]);
					if(opt[i]=='-') printf("subq %s, %s\n",posChar[1],posChar[2]);
					if(opt[i]=='*') printf("imulq %s, %s\n",posChar[1],posChar[2]);
//					if(printf(""))
				} else {
					printf("movq %s, %%rax\n",posChar[2]);
					printf("idivq %s\n",posChar[1]);
					printf("movq %%rax\n, %s",posChar[2]);
				}
				printf("xorq %s,%%r15\n",posChar[2]);
				j=17;
			}
		}
	}
	puts("movq %r15, %rax");
	puts("subq    $20000032,%rbp"),puts("popq    %r15");
	puts("popq    %r14"),puts("popq    %r13"),puts("popq    %r12");
	puts("popq    %rbp"),puts("popq    %rsp"),puts("popq    %rbx");
}