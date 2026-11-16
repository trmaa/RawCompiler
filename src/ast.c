#include <stdio.h>
#include "ast.h"

ast_t *
new_ast_vec(int size)
{
	ast_hdr *h = malloc(size * sizeof (ast_t) + sizeof (ast_hdr));
	h->size = 0;
	h->capa = size;
	return ast_vec(h);
}

void
free_astv(ast_t *v)
{
	ast_hdr *h = ast_hdr(v);
	int size = h->size;
	int i;

	for (i = 0; i < size; i++) {
		free_ast(v[i]);
	}

	free(h);
}

ast_t *
push_ast(ast_t *v, ast_t n)
{
	ast_hdr *h = ast_hdr(v);

	h->size++;
	int size = h->size;
	int capa = h->capa;

	if (size >= capa) {
		h->capa *= 2;
		h = realloc(h, h->capa * sizeof (ast_t) + sizeof (ast_hdr));
		v = ast_vec(h);
		v[size - 1] = n;
		return v;
	}

	v[size - 1] = n;
	return v;
}

void
free_ast(ast_t ast)
{
	if (ast.args) {
		free_astv(ast.args);
	}

	/* there is only one string sins its a union */
	if (ast.val) {
		free(ast.val);
	}
}

static void
print_ast_aux(ast_t *root, int nest)
{
	int i, size;

	printf("\x1b[32m[%02d]\x1b[33m", root->type);/**/

	if (nest > 0) {
		for (i = 0; i < nest - 1; i++)
			printf("        ");
		putchar('L');
		for (i = 1; i < 7; i++)
			putchar('_');
		putchar(' ');
	}

	printf("\x1b[36m");

	if (root->args)
		size = ast_hdr(root->args)->size;

	if (root->type == AEND) {
		printf("END\n");
	}
	else if (root->type == AID) {
		printf("ID(%s)\n", root->val);
	}
	else if (root->type == ALIT) {
		printf("LIT(%s)\n", root->val);
	}
	else if (root->type == ATYPE) {
		printf("TYPE(%d)\n", root->tok->type);
	}
	else if (root->type == AOP) {
		printf("OP(%d):\n", root->tok->type);
		for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i], nest + 1);
		}
	}
	else if (root->type == AEXPR) {
		printf("EXPR:\n");
		for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i], nest + 1);
		}
	}
	else if (root->type == AGLOBL) {
		printf("GLOBL:\n");
		for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i], nest + 1);
		}
	}
	else if (root->type == APUB) {
		printf("PUB:\n");
		for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i], nest + 1);
		}
	}
	else if (root->type == AEQ) {
		printf("=\n");
		for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i], nest + 1);
		}
	}
	else if (root->type == AFN) {
		printf("FN:\n");
		/*for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i]);
		}*/
	}
	else if (root->type == AROOT) {
		printf("ROOT:\n");
		for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i], nest + 1);
		}
	}
	else {
		printf("TOK(%d):\n", root->type);
		for (i = 0; i < size; i++) {
			print_ast_aux(&root->args[i], nest + 1);
		}
	}

	printf("\x1b[0m");

}

void
print_ast(ast_t *root)
{
	print_ast_aux(root, 0);
}
