#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LENGTH_OF_CASE 60
#define LENGTH_OF_CHAR 10
#define LENGTH_NODE 1000
#define SIZE_OF_CHAR 100
#define REG_NUM 5
#define REG_LEN 5
#define LENGTH_OF_BYTE 8

enum{
	ADD = '+',
	SUBTRACT = '-',
	MULTIPLY = '*',
	DIVIDE = '/'
};

char REG[REG_NUM][REG_LEN] = {"%rdi", "%rsi", "%rcx", "%r8", "%r9"};
char RAX[REG_LEN] = {"%rax"}, RCX[REG_LEN] = {"%rcx"}, R10[REG_LEN] = {"%r10"}, R11[REG_LEN] = {"%r11"};
char RSP[REG_LEN] = {"%rsp"};
int children[LENGTH_NODE][SIZE_OF_CHAR]; // use of trie
int node_index[LENGTH_NODE];

int node_cnt = 0;
int new_node() {
	++node_cnt;
	for(int i = 0; i < SIZE_OF_CHAR; i++) {
		children[node_cnt][i] = 0;
	}
	return node_cnt;
}

int index_clock = 0;
int get_index(char* str) {
	int len = strlen(str);
	int current_node = 0;
	for(int i = 0; i < len; i++) {
		int current_char = str[i] - 'a';
		if(!children[current_node][current_char]) children[current_node][current_char] = new_node();
		current_node = children[current_node][current_char];
	}
	if(!node_index[current_node]) node_index[current_node] = ++index_clock;
	return node_index[current_node];
}

void init() {
	get_index("a1");
	get_index("a2");
	get_index("a3");
	printf("addq $-400, %%rsp\n"); // initialize the location of %rsp

	printf("movq %%rdx, %%rcx\n"); // rcx = rdx to leave out the space of %rdx so that we can do division
	printf("movq $0, %%r11\n");
}

void endit() {
	printf("addq $400, %%rsp\n");
	printf("movq %s, %s\n", R11, RAX); // rax = r11
	printf("ret\n");
}

bool is_digit(char* str) {
	int len = strlen(str);
	int i = 0;
	if(str[i] == '-') i++;
	for(; i < len ; i ++) {
		char current_char = str[i];
		if(current_char < '0' || current_char >'9') return false;
	}
	return true;
}
int get_number(char* str) {
	int len = strlen(str), result = 0;
	int i = 0, sign = 1;
	if(str[i] == '-') {
		i++;
		sign = -1;
	}
	for(; i < len; i++) {
		char current_char = str[i] - '0';
		result = result * 10 + current_char;
	}
	return result * sign;
}

void convert_to_assemble(int operant_1, int operant_2, int operant_2_number, char * operator, int operant_var);

void add(int operant_1, int operant_2, int operant_2_num);
void subtract(int operant_1, int operant_2, int operant_2_num);
void multiply(int oeprant_1, int operant_2, int operant_2_num);
void divide(int operant_1, int operant_2, int operant_2_num);

void input_and_convert_to_assemble() {

	char operant_1[LENGTH_OF_CHAR], operant_2[LENGTH_OF_CHAR], operator[LENGTH_OF_CHAR];
	char operant_variable[LENGTH_OF_CHAR];
	scanf("%s%*s", operant_variable);
	int operant_var_index = get_index(operant_variable);
	scanf("%s%s%s", operant_1, operator, operant_2);

	int operant_2_length = strlen(operant_2);
	operant_2[operant_2_length - 1] = '\0';

	int operant_1_index = get_index(operant_1);
	int operant_2_index = 0, operant_2_number = 0;

	if(!is_digit(operant_2)) {
		operant_2_index = get_index(operant_2);
	} else {
		operant_2_number = get_number(operant_2);
	}

	convert_to_assemble(operant_1_index, operant_2_index, operant_2_number, operator, operant_var_index);
}

void print_register(int operant) {
	if(operant <= 5) printf("%s", &REG[operant - 1][0]);
	else printf("%d(%s)", 400 - (operant - 5) * LENGTH_OF_BYTE, RSP);
}

void add(int operant_1, int operant_2, int operant_2_number) {

	printf("movq ");
	print_register(operant_1);
	printf(", %s\n", RAX);

	if(!operant_2) {
		printf("addq $%d, %s\n", operant_2_number, RAX);
	} else {
		printf("addq ");
		print_register(operant_2);
		printf(", %s\n", RAX);
	}
}

void subtract(int operant_1, int operant_2, int operant_2_number) {

	printf("movq ");
	print_register(operant_1);
	printf(", %s\n", RAX);

	if(!operant_2) {
		printf("subq $%d, %s\n", operant_2_number, RAX);
	} else {
		printf("subq ");
		print_register(operant_2);
		printf(", %s\n", RAX);
	}
}

void multiply(int operant_1, int operant_2, int operant_2_number) {
	printf("movq ");
	print_register(operant_1);
	printf(", %s\n", RAX);

	if(!operant_2) {
		printf("imulq $%d, %s\n", operant_2_number, RAX);
	} else {
		printf("imulq ");
		print_register(operant_2);
		printf(", %s\n", RAX);
		puts("");
	}
}

void divide(int operant_1, int operant_2, int operant_2_number) {
	
	printf("movq ");
	print_register(operant_1);
	printf(", %s\n", RAX);

	printf("cqto\n");

	if(!operant_2) {
		printf("movq $%d, %s\n", operant_2_number, R10);
		printf("idivq %s\n", R10);
	} else {
		printf("idivq ");
		print_register(operant_2);
		puts("");
	}
}

void convert_to_assemble(int operant_1, int operant_2, int operant_2_number, char* operator, int operant_var) {
	if(operator[0] == ADD) add(operant_1, operant_2, operant_2_number);
	if(operator[0] == SUBTRACT) subtract(operant_1, operant_2, operant_2_number);
	if(operator[0] == MULTIPLY) multiply(operant_1, operant_2, operant_2_number);
	if(operator[0] == DIVIDE) divide(operant_1, operant_2, operant_2_number);

	printf("movq %s, ", RAX);
	print_register(operant_var);
	printf("\n");

	printf("xorq %s, %s\n", RAX, R11);
}

int main() {

	init();
	
	int kase = LENGTH_OF_CASE;
	while(kase--) {
		input_and_convert_to_assemble();
		puts("");
	}

	endit();

	return 0;
}
