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

void
print_ast(ast_t *root, int nest)
{
	int i;

	/*printf("[%d]", root->type);*/

	for (i = 0; i < nest; i++)
		putchar('\t');

	if (root->type == AEND) {
		printf("END\n");
	}
	else if (root->type == AID) {
		printf("ID(%s)\n", root->val);
	}
	else if (root->type == AINTL || root->type == ACHARL
		|| root->type == ASTRL || root->type == AFLOATL) {
		printf("%s\nÂ", root->val);
	}
	else if (root->type == ATYPE) {
		printf("TYPE(%d)\n", root->data_type);
	}
	else if (root->type == AGLOBL) {
		printf("GLOBL:\n");
		for (i = 0; i < ast_hdr(root->args)->size; i++) {
			print_ast(&root->args[i], nest + 1);
		}
		putchar('\n');
	}
	else if (root->type == APUB) {
		printf("PUB:\n");
		for (i = 0; i < ast_hdr(root->args)->size; i++) {
			print_ast(&root->args[i], nest + 1);
		}
		putchar('\n');
	}
	else if (root->type == AEQ) {
		printf("=\n");
		for (i = 0; i < ast_hdr(root->args)->size; i++) {
			print_ast(&root->args[i], nest + 1);
		}
		putchar('\n');
	}
	else if (root->type == AFN) {
		printf("FN:\n");
		/*for (i = 0; i < ast_hdr(root->args)->size; i++) {
			print_ast(&root->args[i]);
		}*/
		putchar('\n');
	}
	else if (root->type == AROOT) {
		printf("ROOT:\n");
		for (i = 0; i < ast_hdr(root->args)->size; i++) {
			print_ast(&root->args[i], nest + 1);
		}
		putchar('\n');
	}
	else {
		printf("TOK(%d):\n", root->type);
		for (i = 0; i < ast_hdr(root->args)->size; i++) {
			print_ast(&root->args[i], nest + 1);
		}
		putchar('\n');
	}
}
