#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 100010
#define pos(cnt,x) pos([flag[x][cnt]])
#define Reg_Size 3
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
int son[2][N],flag[2][N],last[4][N],cntt[3][100][100];
int *pos[4][N];
char opt[N],out[4]={'0','a','v'};
int jc[17],mem[N],*ms,**pjc[17];
int cntjc[17][100];
int isReg(int *x) {
	if((x-jc)/sizeof(int)<=Reg_Size&&(x-jc)>=0) return 1;
	return 0;
}
void modify(int **s) {
	s=&ms;
}
int main() {
	freopen("../testdata/10.txt","r",stdin);
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
			printf("reg%d = a%d;\n",i,i);
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
			printf("mem[%ld] = reg%d;\n",ms-mem,tmp);
			ms++;
			jc[tmp]=0;
		}
		for(int j=1;j<=Reg_Size;j++) {
			if(jc[j]==0) {
				jc[j]=last[2][i];
				pos[2][i]=&jc[j];
				pjc[j]=&pos[2][i];
				memcpy(cntjc[j],cntt[2][i],sizeof(int)*Max);
				printf("reg%d = ",j);
				if(flag[0][i]==3) {
					printf("%d;\n",son[0][i]);
				} else {
//					printf("%d %d\n",flag[0][i],son[0][i]);
					int *pos1=pos[flag[0][i]][son[0][i]];
					if(isReg(pos1)) printf("reg%ld;\n",pos1-jc);
					else printf("mem[%ld];\n",pos1-mem);
				}
				printf("reg%d %c= ",j,opt[i]);
				if(flag[1][i]==3) {
					printf("%d;\n",son[1][i]);
				} else {
					int *pos2=pos[flag[1][i]][son[1][i]];
					if(isReg(pos2)) printf("reg%ld;\n",pos2-jc);
					else printf("mem[%ld];\n",pos2-mem);
				}
				
				printf("if(reg%d!=v[%d]) printf(\"Error\");\n",j,i);
				j=17;
			}
		}
	}
}