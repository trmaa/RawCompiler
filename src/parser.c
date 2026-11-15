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

#define log_tok()	\
	do {	\
		fprintf(stderr, "Tok (%d):\t", tok->type);	\
		fflush(stderr);	\
		write(2, tok->start, tok->len);	\
		write(2, "\n", 1);	\
	} while (0)

ast_t parse_globl();
ast_t parse_fn();

ast_t *
parse(struct token *toks)
{
	for (tok = toks; tok->type != TEND; tok++)
		log_tok();

	tok = toks;

	root.type = AROOT;
	root.tok = NULL;

	root.args = ast_vec(10);

	for (; tok->type != TEND;) {
		if (tok->type == TLET) push(root.args, parse_globl());
		else if (tok->type == TFN) push(root.args, parse_fn());
		else {
			unexpected();
		}
	}

	return &root;
}

ast_t
parse_globl()
{
	ast_t t;
	t.type = AEND;

	for (; tok->type != TSEMI; tok++);
	tok++;

	return t;
}

ast_t
parse_fn()
{
	int nest = 1;

	ast_t t;
	t.type = AEND;

	for (; tok->type != TLBK; tok++)
		log_tok();
	log_tok();
	tok++;
	log_tok();

	while (nest > 0) {
		fprintf(stderr, "nesting: %d => ", nest);
		log_tok();
		tok++;
		if (tok->type == TLBK)
			nest++;
		else if (tok->type == TRBK)
			nest--;
	}
	log_tok();
	tok++;
	log_tok();

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
