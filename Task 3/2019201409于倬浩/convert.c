#include <stdio.h>
#define INPUT_FILE_NAME "1.txt"
int main(int argc, char *argv[]) {
	freopen("output2.c", "w", stdout);
	if(argc == 1) freopen("../testdata/" INPUT_FILE_NAME, "r", stdin);
	else {
		char filename[256];
		strcat(filename, "../testdata/");
		strcat(filename, argv[1]);
		freopen(filename, "r", stdin);
	}
	char str[256];
	puts("#include <stdio.h>");
	puts("int main() {");
	printf("    freopen(\"result2.txt\", \"w\", stdout);\n");
	printf("    int a1 = 998, a2 = 244, a3 = 353;\n    int");
	for(int i = 1; i <= 60; ++i) printf(i == 60 ? "v%d;\n" : " v%d,", i);
	int x = 0;
	while(fgets(str, 255, stdin)) {
		printf("    "), puts(str);
		++x;
		if(x > 60) break;
		printf("    printf(\"%%d\\n\", v%d);\n", x);
	}
	printf("    return 0; \n}");
}