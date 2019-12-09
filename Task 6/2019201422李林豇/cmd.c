#include"cmd.h"

void cmd_help(void)
{
	puts("Please put the arguments as follows:");
	puts("./cal_jose [loop lenth] [cw(clockwise) or aw(anticlockwise)] [gap lenth]");
}

unsigned char check_digit(char* arg)
{
	int len = strlen(arg);
	for(int i = 0;i < len;i++)
		if(arg[i] > '9'||arg[i] < '0')
			return FALSE;
	return TRUE;
}

unsigned char check_direct(char* arg)
{
	int len = strlen(arg);
	if(len != LEN_OF_SECOND)
		return FALSE;
	if(arg[0]=='a' && arg[1]=='w')
		return TRUE;
	if(arg[0]=='c' && arg[1]=='w')
		return TRUE;
	return FALSE;
}

unsigned char check_cmd(int argv, char* arg[])
{
	if(argv != ARG_NUM)
	{
		if(argv > ARG_NUM)
			puts("Too many arguments in one line");
		else
			puts("Too few arguments in one line");
		cmd_help();
		return FALSE;
	}
	unsigned char flag = 1;
	flag &= check_digit(arg[1]);
	flag &= check_direct(arg[2]);
	flag &= check_digit(arg[3]);
	if(!flag)
	{
		puts("Wrong arguments put");
		cmd_help();
	}
	return flag;
}