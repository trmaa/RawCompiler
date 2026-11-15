#include <stdio.h>
#include "tok.h"

static char *special = "X()[]{};:,-+/*<>=%&|^~\"\'!@";
static char *white = "X \n\t";

static struct token tokens[1024];

int line = 1, col = -1;
int cid, tid = 0, bid = 0, i;
char c, buf[32];

static int
char_in(char c, char *s)
{
	int i;
	for (i = 0; s[i] != 0; i++)
		if (c == s[i])
			return i;

	return 0;
}

struct token *
tokenize(char *code)
{
	buf[0] = 0;

	for (cid = 0; code[cid] != 0; cid++) {
		c = code[cid];
		col++;

		if (c == '\\') {
			cid++;
			continue;
		}

		if (c == '/' && code[cid + 1] == '/') {
			line++;
			col = -1;

			while (code[++cid] != '\n');

			continue;
		}

		if (!strcmp(buf, "pub")) tokens[tid].type = TPUB;
		else if (!strcmp(buf, "fn")) tokens[tid].type = TFN;
		else if (!strcmp(buf, "let")) tokens[tid].type = TLET;
		else if (!strcmp(buf, "if")) tokens[tid].type = TIF;
		else if (!strcmp(buf, "else")) tokens[tid].type = TELSE;
		else if (!strcmp(buf, "for")) tokens[tid].type = TFOR;
		else if (!strcmp(buf, "type")) tokens[tid].type = TTYPE;
		else if (!strcmp(buf, "and")) tokens[tid].type = TAND;
		else if (!strcmp(buf, "or")) tokens[tid].type = TOR;
		else if (!strcmp(buf, "not")) tokens[tid].type = TNOT;
		else if (!strcmp(buf, "int")) tokens[tid].type = TWORD;
		else if (!strcmp(buf, "word")) tokens[tid].type = TWORD;
		else if (!strcmp(buf, "integer")) tokens[tid].type = TWORD;
		else if (!strcmp(buf, "u32")) tokens[tid].type = TWORD;
		else if (!strcmp(buf, "float")) tokens[tid].type = TFLOAT;
		else if (!strcmp(buf, "f32")) tokens[tid].type = TFLOAT;
		else if (!strcmp(buf, "real")) tokens[tid].type = TFLOAT;
		else if (!strcmp(buf, "char")) tokens[tid].type = TBYTE;
		else if (!strcmp(buf, "byte")) tokens[tid].type = TBYTE;
		else if (!strcmp(buf, "u8")) tokens[tid].type = TBYTE;
		else if (!strcmp(buf, "arr")) tokens[tid].type = TARR;
		else if (!strcmp(buf, "array")) tokens[tid].type = TARR;
		else if (!strcmp(buf, "farr")) tokens[tid].type = TFARR;
		else if (!strcmp(buf, "float_array")) tokens[tid].type = TFARR;
		else if (!strcmp(buf, "sstr")) tokens[tid].type = TSSTR;
		else if (!strcmp(buf, "stack_string")) tokens[tid].type =TSSTR;
		else if (!strcmp(buf, "vec")) tokens[tid].type = TVEC;
		else if (!strcmp(buf, "int_ptr")) tokens[tid].type = TVEC;
		else if (!strcmp(buf, "fvec")) tokens[tid].type = TFVEC;
		else if (!strcmp(buf, "float_ptr")) tokens[tid].type = TFVEC;
		else if (!strcmp(buf, "str")) tokens[tid].type = TSTR;
		else if (!strcmp(buf, "string")) tokens[tid].type = TSTR;
		else if (!strcmp(buf, "sp")) tokens[tid].type = TSP;
		else if (!strcmp(buf, "bp")) tokens[tid].type = TBP;
		else if (!strcmp(buf, "arg")) tokens[tid].type = TARG;
		else if (!strcmp(buf, "sys")) tokens[tid].type = TSYS;
		else if (!strcmp(buf, "eax")) tokens[tid].type = TEAX;
		else if (!strcmp(buf, "ebx")) tokens[tid].type = TEBX;
		else if (!strcmp(buf, "ecx")) tokens[tid].type = TECX;
		else if (!strcmp(buf, "edx")) tokens[tid].type = TEDX;
		else if (!strcmp(buf, "ret")) tokens[tid].type = TRET;
		else if (!strcmp(buf, "push")) tokens[tid].type = TPUSH;
		else if (!strcmp(buf, "pop")) tokens[tid].type = TPOP;
		else if (!strcmp(buf, "size")) tokens[tid].type = TSIZEOF;
		else {
			if (char_in(c, white)) {
				if (bid)
					tok_lit(code);
				goto white;
			}
			if (char_in(c, special)) {
				if (bid)
					tok_lit(code);
				goto special;
			}

			buf[bid++] = c;
			buf[bid] = 0;
			continue;
		}

		tokens[tid].start = &code[cid - bid];
		tokens[tid].len = bid;
		tokens[tid].line = line;
		tokens[tid].col = col - bid;
		tid++;
		bid = 0;
		buf[0] = 0;
		cid--;

		continue;

	white:
		if (c == '\n') {
			line++;
			col = -1;
		}

		while (char_in(code[cid + 1], white)) {
			if ((c = code[++cid]) == '\n') {
				line++;
				col = -1;
			}
		}

		continue;

	special:
		switch (c) {
		case 'X':
			break;
		default:
			tok_sp(code);
		}

		cid--;
		col--;

		continue;
	}

	tokens[tid].type = TEND;

	return tokens;
}

void
tok_lit(char *code)
{
	int st = cid - bid;
	tokens[tid].start = &code[cid - bid];
	tokens[tid].len = bid;
	tokens[tid].line = line;
	tokens[tid].col = col;

	if (char_in(code[cid - bid], "X0123456789")) {
		tokens[tid].type = TINTL;

		while (char_in(code[st], "X0123456789"))
			st++;

		if (code[st] == '.')
			tokens[tid].type = TFLOATL;
	} else {
		tokens[tid].type = TID;
	}

	tid++;

	bid = 0;
	buf[0] = 0;
}

void
tok_sp(char *code)
{
	tokens[tid].len = 1;
	tokens[tid].start = &code[cid];

	switch (c) {
	case '(': tokens[tid].type = TLP; break;
	case ')': tokens[tid].type = TRP; break;
	case '[': tokens[tid].type = TLBC; break;
	case ']': tokens[tid].type = TRBC; break;
	case '{': tokens[tid].type = TLBK; break;
	case '}': tokens[tid].type = TRBK; break;
	case ';': tokens[tid].type = TSEMI; break;
	case ',': tokens[tid].type = TCMA; break;
	case '@': tokens[tid].type = TREF; break;
	case '%': tokens[tid].type = TMOD; break;
	case '+': switch (code[cid + 1]) {
		  case '+': tokens[tid].len++; cid++; col++;
				tokens[tid].type = TINC; break;
		  case '=': tokens[tid].len++; cid++; col++;
				tokens[tid].type = TADDI; break;
		  default: tokens[tid].type = TADD; break; } break;
	case '-': switch (code[cid + 1]) {
		  case '-': tokens[tid].len++; cid++; col++;
				tokens[tid].type = TDEC; break;
		  case '=': tokens[tid].len++; cid++; col++;
				tokens[tid].type = TSUBI; break;
		  default: tokens[tid].type = TSUB; break; } break;
	case '*': switch (code[cid + 1]) {
		  case '=': tokens[tid].len++; cid++; col++;
				tokens[tid].type = TMULTI; break;
		  default: tokens[tid].type = TMULT; break; } break;
	case '/': switch (code[cid + 1]) {
		  case '=': tokens[tid].len++; cid++; col++;
				tokens[tid].type = TDIVI; break;
		  default: tokens[tid].type = TDIV; break; } break;
	case '>': switch (code[cid + 1]) {
		  case '>': tokens[tid].len++; cid++; col++;
			switch (code[cid + 1]) {
			case '=': tokens[tid].len++; cid++;
				col++; tokens[tid].type = TSHRI; break;
			default: tokens[tid].type = TSHR; break; } break;
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TGE; break;
		  default: tokens[tid].type = TGT; break; } break;
	case '<': switch (code[cid + 1]) {
		  case '<': tokens[tid].len++; cid++; col++;
			switch (code[cid + 1]) {
			case '=': tokens[tid].len++; cid++; col++;
				tokens[tid].type = TSHLI; break;
			default: tokens[tid].type = TSHL; break; } break;
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TLE; break;
		  default: tokens[tid].type = TLT; break; } break;
	case '=': switch (code[cid + 1]) {
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TEE; break;
		  default: tokens[tid].type = TEQ; break; } break;
	case '!': switch (code[cid + 1]) {
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TNE; break;
		  default: tokens[tid].type = TNOT; break; } break;
	case '~': switch (code[cid + 1]) {
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TBNOTI; break;
		  default: tokens[tid].type = TBNOT; break; } break;
	case '^': switch (code[cid + 1]) {
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TXORI; break;
		  default: tokens[tid].type = TXOR; break; } break;
	case '&': switch (code[cid + 1]) {
		  case '&': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TAND; break;
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TBANDI; break;
		  default: tokens[tid].type = TBAND; break; } break;
	case '|': switch (code[cid + 1]) {
		  case '|': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TOR; break;
		  case '=': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TBORI; break;
		  default: tokens[tid].type = TBOR; break; } break;
	case ':': switch (code[cid + 1]) {
		  case ':': tokens[tid].len++; cid++; col++;
			tokens[tid].type = TDOT; break;
		  default: tokens[tid].type = TKTL; break; } break;
	case '\"': tokens[tid].type = TSTRL; goto slit;
	case '\'': tokens[tid].type = TCHARL; goto clit;
	}

	tokens[tid].line = line;
	tokens[tid].col = col++;

	cid++;
	tid++;
	return;

slit:
	tokens[tid].line = line;
	tokens[tid].col = col;
	tokens[tid].start = &code[cid];
	cid++;
	for (tokens[tid].len = 2; code[cid++] != '\"'; tokens[tid].len++) col++;
	tid++;
	return;

clit:
	tokens[tid].line = line;
	tokens[tid].col = col;
	tokens[tid].start = &code[cid];
	cid++;
	for (tokens[tid].len = 2; code[cid++] != '\''; tokens[tid].len++) col++;
	tid++;
	return;
}
