#ifndef conf_h
#define conf_h

#define conf_cc_flags	"-w", "-Wno-implicit-int", \
			"-Wno-implicit-function-declaration", \
			"-Wno-builtin-declaration-mismatch", \
			"-Wno-incompatible-pointer-types", \
			"-Wno-int-conversion"

enum step {
	CMP = 1,
	AS = 2,
	LD = 4
};

enum debug {
	PREP = 1,
	TOKS = 2,
	AST = 4,
	CODE = 8
};

struct conf {
	enum step mode;
	enum debug logs;
	char *out_path;
};

extern struct conf conf;

#endif
