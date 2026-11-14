#ifndef tok_h
#define tok_h

enum tok {
	EOF = -1
};

struct token {
	enum tok type;
	char *cont;
};

#endif
