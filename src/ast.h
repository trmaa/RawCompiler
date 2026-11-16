#ifndef ast_h
#define ast_h

#include "tok.h"

enum ast_type {
	AEND,

	APUB,

	AEXPR, AOP,

	AGLOBL, AID, ATYPE, AEQ, ALIT,

	AFN,

	AROOT
};

typedef struct ast_node {
	enum ast_type type;
	struct token *tok;

	/* heap!!! */
	struct ast_node *args;
	char *val;
} ast_t;

typedef struct astvec_hdr {
	int size, capa;
} ast_hdr;

#define ast_hdr(v) ((ast_hdr *)v - 1)
#define ast_vec(h) ((ast_t *)&h[1])

#endif
