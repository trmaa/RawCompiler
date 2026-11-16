#include <getopt.h>
#include "conf.h"

struct conf conf;

int
config(int n, char **v)
{
	char *sops = "cEho:Sv";
	struct option lops[] = {
		{ "assemble", 0, 0, 'c' },
		{ "preprocess", 0, 0, 'E' },
		{ "help", 0, 0, 'h' },
		{ "out", 1, 0, 'o' },
		{ "compile", 0, 0, 'S' },
		{ "version", 0, 0, 'v' },
		{ 0, 0, 0, 0 }
	};
	int opt;

	conf.mode = CMP | AS | LD;
	conf.out_path = "prog";

	while ((opt = getopt_long(n, v, sops, lops, 0)) != -1) {
		switch (opt) {
		case 'c':
			conf.mode &= ~LD;
			break;
		case 'E':
			conf.mode = 0;
			break;
		case 'h':
			help();
			exit(1);
		case 'o':
			conf.out_path = optarg;
			break;
		case 'S':
			conf.mode = CMP;
			break;
		case 'v':
			version();
			exit(1);
		case '?':
		default:
			exit(66);
		}
	}

	return optind;
}
