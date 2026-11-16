void
help()
{
	puts("RC -- Compiles .raw files into elf32 binaries.");
	puts("Usage: rc [opts] [source files]");
	puts("to produce a binary. (more help on '$ man rc')");
	puts("Options:");
	puts("\t--assemble, -c\n\t\tJust produce a .o file.");
	puts("\t--preprocess, -E\n\t\tJust preprocess a .raw file into a .i.");
	puts("\t--help, -h\n\t\tPrint this message.");
	puts("\t--out, -o [path. default: prog]\n\t\tSet the binary's name.");
	puts("\t--compile, -S\n\t\tJust produce a .s file.");
	puts("\t--version, -v\n\t\tTell the program's version.");
	exit(0);
}

void
version(void)
{
	puts(	"Raw compiler (rc)"
		" -- Copyright (C) 2026 Pablo T.");
	puts("Version: dev0.0.0 (doing AST)");
	puts("License: GPL 2.0");
	putchar('\n');
	puts("Tutorial at:");
	puts("\t$ rc --help\tor\t$ man rc");
	exit(0);
}
