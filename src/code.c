#include "ast.h"

int
code_gen(ast_t *root, char *code)
{
	print_ast(root);

	strcpy(code,
		".globl main\n"
		"main:\n"
		"mov $69, %eax\n"
		"ret\n");

	return strlen(code);
}
