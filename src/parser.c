#include <stdio.h>
#include "ast.h"
#include "tok.h"
#include "err.h"

struct token *tok;
ast_t root = {0};

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
		fprintf(stderr, "[%d](%d):\t", tok->type, tok->line);	\
		fflush(stderr);	\
		write(2, tok->start, tok->len);	\
		write(2, "\n", 1);	\
	} while (0)

#define log_tokens()	\
	do {	\
		for (tok = toks; tok->type != TEND; tok++)	\
			log_tok();	\
	} while (0)

ast_t parse_globl();
ast_t parse_fn();

ast_t *
parse(struct token *toks)
{
	tok = toks;

	root.type = AROOT;

	root.args = new_ast_vec(10);

	for (; tok->type != TEND;) {
		if (tok->type == TLET) root.args = push_ast(root.args, parse_globl());
		else if (tok->type == TFN) root.args = push_ast(root.args, parse_fn());
		else {
			unexpected();
		}
	}

	return &root;
}

ast_t
parse_globl()
{
	ast_t a = {0}, b = {0}, c = {0}, d = {0};
	a.type = AGLOBL;

	tok++;

	if (tok->type == TPUB) {
		b.pub = 1;
		tok++;
	} else {
		b.pub = 0;
	}

	if (tok->type != TID)
		unexpected();

	b.type = AID;
	b.val = malloc((1 + tok->len) * sizeof (char));
	sprintf(b.val, "%.*s\0", tok->len, tok->start);

	a.args = new_ast_vec(3);
	a.args = push_ast(a.args, b);

	tok++;

	if (tok->type == TEQ) {
		c.type = AEQ;

		tok++;

		if (tok->type == TINTL) {
			d.type = AINTL;
		}
		else if (tok->type == TFLOATL) {
			d.type = AFLOATL;
		}
		else if (tok->type == TCHARL) {
			d.type = ACHARL;
		}
		else if (tok->type == TSTRL) {
			d.type = ASTRL;
		}
		else {
			unexpected();
		}

		d.val = malloc((1 + tok->len) * sizeof (char));
		sprintf(d.val, "%.*s\0", tok->len, tok->start);

		c.args = new_ast_vec(2);
		c.args = push_ast(c.args, d);
	}
	else {
		if (tok->type != TKTL)
			unexpected();

		tok++;

		if (tok->type == TWORD) {
			c.type = ATYPE;
			c.data_type = DWORD;
		} else {
			unexpected();
		}
	}

	a.args = push_ast(a.args, c);

	tok++;

	if (tok->type != TSEMI)
		unexpected();

	tok++;

	return a;
}

ast_t
parse_fn()
{
	int nest = 1;

	ast_t t = {0};
	t.type = AFN;

	for (; tok->type != TLBK; tok++);
	tok++;

	while (nest > 0) {
		tok++;
		if (tok->type == TLBK)
			nest++;
		else if (tok->type == TRBK)
			nest--;
	}
	tok++;

	return t;
}
