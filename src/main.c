/*
 * Raw Compiler (rc) --
 *
 * It's a compiler for the Raw programming language.
 *
 * This language is lower level than C. Think of it as a premium,
 * cross-platform, modern, high level, assembly language.
 *
 * Copyright (C) Pablo Trik Marin 2026
 * License: GPL 2.0
 */
#include <stdio.h>
#include "conf.h"
#include "err.h"

main(int args_len, char **args)
{
	int fid = 0;

	if (args_len < 2) {
		version();
		exit(1);
	}

	fid = config(args_len, args);

	mkdir(".tmp", 0777);

	for (; fid < args_len; fid++) {
		comp(args[fid]);
	}

	if (conf.mode & LD) {
		link();
	}

	execlp("rm", "rm", "-rf", ".tmp", 0);
}

void
comp(char *src_path)
{
	enum err st;
	char *name, *ext;
	char prep_path[32], code_path[32], obj_path[32], out_path[32];

	if ((name = strrchr(src_path, '/')) == NULL)
		name = src_path;
	else
		name++;

	if ((ext = strrchr(src_path, '.')) == NULL)
		fatalf(USER_ERR, "Source files must have an extension.\n");

	sprintf(prep_path, ".tmp/%s.i", name);
	sprintf(code_path, ".tmp/%s.s", name);
	sprintf(obj_path, ".tmp/%s.o", name);
	strcpy(out_path, conf.out_path);

	/* PREPROCESS */
	if (!strcmp(ext, ".raw")) {
		if (!fork()) {
			execlp("cc", "cc", "-E", "-P", "-traditional-cpp",
				"-nostdinc", "-x", "c",
				src_path, "-o", prep_path, 0);
		} else {
			wait(&st);
			if (st)
				fatalf(USER_ERR,
					"Preprocessing failed (%s).\n",
					src_path);
		}

		if (!(conf.mode & CMP)) {
			sprintf(out_path, "%s.i", name);
			cp(prep_path, out_path);
			return;
		}

		if (conf.logs & PREP) {
			cat(prep_path);
		}

		comp(prep_path);
		return;
	}

	/* COMPILE */
	else if (!strcmp(ext, ".i")) {
		compile(src_path, code_path);

		if (!(conf.mode & AS)) {
			sprintf(out_path, "%s.s", name);
			cp(code_path, out_path);
			return;
		}

		comp(code_path);
		return;
	}

	else if (!strcmp(ext, ".c")) {
		if (!fork()) {
			execlp("cc", "cc", "-S", "-x", "c", conf_cc_flags, src_path,
				"-o", code_path, 0);
		} else {
			wait(&st);
			if (st) {
				fatalf(USER_ERR, "%s's compilation failed.\n",
					src_path);
			}
		}

		if (!(conf.mode & AS)) {
			sprintf(out_path, "%s.s", name);
			cp(code_path, out_path);
			return;
		}

		comp(code_path);
		return;
	}

	/* ASSEMBLE */
	else if (!strcmp(ext, ".s")) {
		if (!(conf.mode & AS))
			return;

		if (!fork()) {
			execlp("cc", "cc", "-c", "-m32", "-x", "assembler",
				src_path, "-o", obj_path, 0);
		} else {
			wait(&st);
			if (st)
				fatalf(USER_ERR, "Assembling failed (%s).\n",
					src_path);
		}

		if (!(conf.mode & LD)) {
			sprintf(out_path, "%s.o", name);
			cp(obj_path, out_path);
			return;
		}
	} else {
		fatalf(USER_ERR, "Unrecognized extension: %s\n", ext);
	}
}

void
link(void)
{
	char cmd[1024];
	sprintf(cmd, "cc .tmp/*.o -o %s -m32 -w", conf.out_path);
	system(cmd);
}
