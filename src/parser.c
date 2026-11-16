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
		fprintf(stderr, "[%02d](%04d): ", tok->type, tok->line);	\
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
ast_t parse_pub();
ast_t parse_aux_bk();
ast_t parse_aux_semi();

ast_t *
parse(struct token *toks)
{
	tok = toks;

	root.type = AROOT;
	root.tok = tok;

	root.args = new_ast_vec(10);

	for (; tok->type != TEND;) {
		if (tok->type == TLET) root.args = push_ast(root.args, parse_globl());
		else if (tok->type == TFN) root.args = push_ast(root.args, parse_fn());
		else if (tok->type == TPUB) root.args = push_ast(root.args, parse_pub());
		else {
			unexpected();
		}
	}

	root.args = push_ast(root.args, (ast_t){ AEND, 0, 0, 0 });

	return &root;
}

ast_t
parse_pub()
{
	ast_t a = {0};
	a.type = APUB;
	a.tok = tok;

	tok++;

	a.args = new_ast_vec(2);

	if (tok->type == TLET) a.args = push_ast(a.args, parse_globl());
	else if (tok->type == TFN) a.args = push_ast(a.args, parse_fn());
	else {
		unexpected();
	}

	return a;
}

ast_t
parse_globl()
{
	ast_t a = {0}, b = {0}, c = {0};
	a.type = AGLOBL;
	a.tok = tok;

	tok++;

	if (tok->type != TID)
		unexpected();

	b.type = AID;
	b.tok = tok;
	b.val = malloc((1 + tok->len) * sizeof (char));
	sprintf(b.val, "%.*s\0", tok->len, tok->start);

	a.args = new_ast_vec(5);
	a.args = push_ast(a.args, b);

	tok++;

	if (tok->type == TKTL) {
		tok++;

		b = (ast_t){0};

		if (tok->type == TWORD || tok->type == TFLOAT
				|| tok->type == TBYTE || tok->type == TARR
				|| tok->type == TFARR || tok->type == TSSTR
				|| tok->type == TVEC || tok->type == TFVEC
				|| tok->type == TSTR) {
			b.type = ATYPE;
		} else {
			unexpected();
		}

		b.tok = tok;

		a.args = push_ast(a.args, b);
		tok++;
	}

	if (tok->type == TEQ) {
		b = (ast_t){0};

		b.type = AEQ;
		b.tok = tok;

		tok++;

		if (tok->type == TINTL) c.type = AINTL;
		else if (tok->type == TFLOATL) c.type = AFLOATL;
		else if (tok->type == TCHARL) c.type = ACHARL;
		else if (tok->type == TSTRL) c.type = ASTRL;
		else unexpected();

		c.tok = tok;
		c.val = malloc((1 + tok->len) * sizeof (char));
		sprintf(c.val, "%.*s\0", tok->len, tok->start);

		b.args = new_ast_vec(2);
		b.args = push_ast(b.args, c);

		a.args = push_ast(a.args, b);
		tok++;
	}

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
	t.tok = tok;

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

ast_t
parse_aux_bk()
{
	int nest = 1;

	ast_t t = {0};
	t.type = AEND;

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

ast_t
parse_aux_semi()
{
	ast_t t = {0};
	t.type = AEND;

	for (; tok->type != TSEMI; tok++);
	tok++;

	return t;
}
