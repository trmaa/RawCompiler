#include "conf.h"
#include "ast.h"

int
code_gen(ast_t *root, char *code)
{
	strcpy(code,
		".globl main\n"
		"main:\n"
		"mov $69, %eax\n"
		"ret\n");

	if (conf.logs & CODE) {
		printf(code);
	}

	return strlen(code);
}
