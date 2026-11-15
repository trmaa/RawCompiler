#ifndef lexer_h
#define lexer_h

enum ast_type {
	AEND,

	AGLOBL, AID, ATYPE, AEQ, AINTL, AFLOATL, ACHARL, ASTRL,

	AFN,

	AROOT
};

enum data_type {
	DWORD, DFLOAT, DBYTE,
	DARR, DFARR, DSSTR,
	DVEC, DFVEC, DSTR
};

typedef struct ast_node {
	enum ast_type type;
	struct ast_node *args;
	char *val;
	char pub;
	enum data_type data_type;
} ast_t;

typedef struct astvec_hdr {
	int size, capa;
} ast_hdr;

#define ast_hdr(v) ((ast_hdr *)v - 1)
#define ast_vec(h) ((ast_t *)&h[1])

void print_ast(ast_t *ast, int nesting);

void free_astv(ast_t *v);
ast_t *new_ast_vec(int size);
ast_t *push_ast(ast_t *v, ast_t n);

#endif
