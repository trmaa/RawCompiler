#include <stdio.h>
#include "ast.h"
#include "tok.h"
#include "err.h"

struct token *tok;
ast_t root;

#define unexpected()	\
	do {	\
		fprintf(stderr, "Unexpected token (%d): ", tok->type);	\
		fflush(stderr);	\
		write(2, tok->start, tok->len);	\
		fprintf(stderr, "\nline: %d : %d\n",	\
				tok->line, tok->col);	\
		exit(USER_ERR);	\
	} while (0)

ast_t parse_globl();
ast_t parse_fn();

ast_t *
parse(struct token *toks)
{
	for (tok = toks; tok->type != TEND; tok++) {
		printf("%d\t->\t %.*s\n", tok->type, tok->len, tok->start);
	}
	fflush(stdout);

	/*
	tok = toks;

	root.type = AROOT;
	root.tok = NULL;

	root.args = ast_vec(10);

	for (;tok->type != TEND;) {
		printf("expecting: %d\n", TLET);
		if (tok->type == TLET) push(root.args, parse_globl());
		else if (tok->type == TFN) push(root.args, parse_fn());
		else {
			unexpected();
		}
	}
	*/

	return &root;
}

ast_t
parse_globl()
{
	ast_t t;
	t.type = AEND;
	return t;
}

ast_t
parse_fn()
{
	ast_t t;
	t.type = AEND;
	return t;
}

ast_t *
ast_vec(int size)
{
	ast_hdr *h = malloc(size * sizeof (ast_t) + sizeof (ast_hdr));
	h->size = size;
	h->capa = size * 2;
	return get_vec(h);
}

void
free_astv(ast_t *v)
{
	free(get_hdr(v));
}

ast_t *
push(ast_t *v, ast_t n)
{
	ast_hdr *h = get_hdr(v);
	int size = ++h->size;
	int capa = h->capa;

	if (size >= capa) {
		h->capa *= 2;
		h = realloc(h, h->capa);
		v = get_vec(h);
		v[size - 1] = n;
		return v;
	}

	v[size - 1] = n;
	return v;
}
