#include <fcntl.h>
#include "tok.h"
#include "ast.h"

static char buf_i[4096];
static char buf_o[4096];

void
compile(char *path_i, char *path_o)
{
	int len_i, len_o;
	int fd_i, fd_o;

	struct token *toks;
	ast_t *ast;

	fd_i = open(path_i, O_RDONLY);
	fd_o = open(path_o, O_WRONLY | O_CREAT | O_TRUNC);

	len_i = read(fd_i, buf_i, 4096);
	buf_i[len_i] = 0;

	toks = tokenize(buf_i);
	ast = parse(toks);
	len_o = code_gen(ast, buf_o);

	free_ast(*ast);

	write(fd_o, buf_o, len_o);

	close(fd_i);
	close(fd_o);
}
