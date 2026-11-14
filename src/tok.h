#ifndef tok_h
#define tok_h

enum tok {
	/* expr */
	TID, TINTL, TFLOATL, TCHARL, TSTRL,

	/* types */
	TWORD, TFLOAT, TBYTE,
	TARR, TFARR, TSSTR,
	TVEC, TFVEC, TSTR,
	TKTL,

	/* keywords */
	TIF, TELSE, TFOR, TLET, TFN, TPUB, TTYPE, TSEMI,

	/* special */
	TLP, TRP, TLBK, TRBK, TLBC, TRBC,
	TCMA, TDOT,

	/* cmp */
	TAND, TOR, TNOT,
	TGT, TLT, TGE, TLE, TEE, TNE,

	/* bit */
	TBAND, TBOR, TXOR, TBNOT,
	TBANDI, TBORI, TXORI, TBNOTI,
	TSHR, TSHL,
	TSHRI, TSHLI,

	/* arithm */
	TINC, TDEC,
	TADD, TSUB, TMULT, TDIV, TMOD,
	TADDI, TSUBI, TMULTI, TDIVI,
	TEQ,

	/* macros */
	TSP, TBP, TARG, TSYS, TSIZEOF, TRET,
	TEAX, TEBX, TECX, TEDX,
	TPUSH, TPOP,
	TREF,

	/* white space */
	TNEWL, TBLANK, TEND
};

struct token {
	enum tok type;
	char *start;
	int len;
	int line, col;
};

#endif
