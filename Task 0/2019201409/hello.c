#include <stdio.h>

int main() {
	printf("听说我要写C语言 还要改码风\n");
	printf("好像是逗号前和大括号前要加空格吧%c%c%c\n", '?', '?', '?');

	int spaces = 1, indent = 4;
	if(indent / spaces == 4) printf("制表符长度是4还可以接受\n");

	for(int i = 1; i <= 2333; ++i) {
		printf("Hello World * %d !\n", i);
	}
	
	return 0;
}