#ifndef lexer_h
#define lexer_h

enum ast_type {
	AROOT, AEND,
};

typedef struct ast_node {
	enum ast_type type;
	struct token *tok;
	struct ast_node *args;
} ast_t;

typedef struct astvec_hdr {
	int size, capa;
} ast_hdr;

#define get_hdr(v) ((ast_hdr *)v - 1)
#define get_vec(h) ((ast_t *)&h[1])

void free_astv(ast_t *v);
ast_t *ast_vec(int size);
ast_t *push(ast_t *v, ast_t n);

#endif
