#include <stdio.h>
#include <stdarg.h>
#include "err.h"

void
fatalf(enum err stat, char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsnprintf(buf, sizeof buf, fmt, args);
	va_end(args);

	fprintf(stderr, "\x1b[31m[fatal]\n%s", buf);

	if (stat == USER_ERR)
		fprintf(stderr, "Use --help to get information.\n");

	fprintf(stderr, "\x1b[0m");

	exit(stat);
}

void
debug(char *txt)
{
	printf("\x1b[33m[log]\n%s\n\x1b[0m", txt);
}

void
todo(char *txt)
{
	printf("\x1b[33m[todo]\n%s\n\x1b[0m", txt);
	exit(ERR);
}
