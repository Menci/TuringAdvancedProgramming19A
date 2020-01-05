#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "errhand.h"

void fatal_error(char *fmt, ...)
{
	va_list argptr;

	va_start(argptr, fmt);
	printf("\n\033[1;31;40m Fatal error: ");
	vprintf(fmt, argptr);
	puts("\033[0m");
	va_end(argptr);
	exit(-1);
}
