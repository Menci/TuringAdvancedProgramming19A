#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
int file_size(char* filename) {
	struct stat statbuf;
	stat(filename,&statbuf);
	int size=statbuf.st_size;
	return size;
}
#define K 64
#define N 2000000
struct Node {
	int left_child, right_child;
	char data[K];
} nodes[N],rec[N]; 
int n,cntt=0;

int writeByte(bool cur,char** code) {
	static int cnt=0,curData=0;
	if(!code) return cnt;
	cnt++,curData<<=1,curData+=cur;
	if(cnt==8) {
		(*(*code))=(char)curData;
		cnt=curData=0;
		(*code)++;
	}
	return cnt;
}
void writeData(int pos,char** code) {
	memcpy(*code, nodes[pos].data, K*sizeof(char)),*code+=K;
	if(nodes[pos].left_child!=-1) writeData(nodes[pos].left_child, code);
	if(nodes[pos].right_child!=-1) writeData(nodes[pos].right_child, code);
}
void writeStruct(int pos,char** code) {
	if(nodes[pos].left_child!=-1&&nodes[pos].right_child!=-1) writeByte(1, code),writeByte(1, code);
	else if(nodes[pos].left_child!=-1) writeByte(1, code),writeByte(0, code);
	else if(nodes[pos].right_child!=-1) writeByte(0, code),writeByte(1, code);
	else writeByte(0, code),writeByte(0, code);
	if(nodes[pos].left_child!=-1) writeStruct(nodes[pos].left_child, code);
	if(nodes[pos].right_child!=-1) writeStruct(nodes[pos].right_child, code);
}
void encode() {
	char* code=malloc(n*(K+20)),*cur=code;
	writeStruct(0,&cur);
	while(writeByte(0, 0)) writeByte(0, &cur);
	writeData(0,&cur);
	FILE* file=fopen("zipped.bin","wb");
	fwrite(code, cur-code, sizeof(char),file);
	fclose(file);
	free(code);
}

int readByte(char** code) {
	static int cnt=-1;
	cnt++;
	if(!code) return (cnt==8)?0:cnt;
	if(cnt==8) (*code)++,cnt=0;
	return (**code)&(1<<(7-cnt));
}
void readStruct(int pos,char **code) {
	static int cnt=0;
	int fl=readByte(code),fr=readByte(code);
	if(fl) rec[pos].left_child=++cnt,readStruct(cnt,code);
	else rec[pos].left_child=-1;
	if(fr) rec[pos].right_child=++cnt,readStruct(cnt,code);
	else rec[pos].right_child=-1;
} 
void readData(int pos,char **code) {
	memcpy(rec[pos].data,*code,K),*code+=K;
	if(rec[pos].left_child!=-1) readData(rec[pos].left_child, code);
	if(rec[pos].right_child!=-1) readData(rec[pos].right_child, code);
}
void decode() {
	FILE* file=fopen("zipped.bin","rb");
	char *mem=malloc(n*(K+10));
	fread(mem, file_size("zipped.bin"), 1, file);
	fclose(file);
	char* code=mem,*cur=code;
	readStruct(0,&cur);
	if(readByte(0)) cur++;
	cur++;
	printf("%ld\n",cur-code);
	readData(0, &cur);
	printf("%ld\n",cur-code);
	fwrite(rec, n, sizeof(struct Node), fopen("unzip.bin","wb"));
	free(mem);
}
int check(int pos1,int pos2) {
	if((pos1==-1)!=(pos2==-1)) return 1;
	if(pos1==-1) return 0;
	for(int i=0;i<K;i++)
		if(nodes[pos1].data[i]!=rec[pos2].data[i]) return 1;
	if(check(nodes[pos1].left_child, rec[pos2].left_child)) return 1;
	if(check(nodes[pos1].right_child, rec[pos2].right_child)) return 1;
	return 0;
}
void init(char fileName[]) {
	FILE* input=fopen(fileName,"rb");
	n=file_size(fileName)/sizeof(struct Node);
	fread(nodes, sizeof(struct Node), n, input);
	fclose(input);
}
int main(int argc, char* argv[])
{
	char file[]="3.in";
	init(file);
	encode();
	decode();
	printf(check(0, 0)?"Wrong Answer\n":"Correct\n");
	printf("%0.2f%%",(double)file_size("zipped.bin")/file_size(file)*100);
}