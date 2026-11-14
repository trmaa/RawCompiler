#include <stdio.h>
#include "lexer.h"
#include "tok.h"

ast_t
parse(struct token *toks)
{
	int i;
	for (i = 0; toks[i].type != TEND; i++) {
		printf("%d (%d, %d)\t\t\t", toks[i].type,
				toks[i].col, toks[i].line);
		fflush(stdout);
		if (toks[i].type != TNEWL && toks[i].type != TBLANK) {
			write(1, toks[i].start, toks[i].len);
			write(1, "\n", 1);
		} else {
			write(1, "\n", 1);
		}
	}

	return -1;
}
