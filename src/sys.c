#include <fcntl.h>
#include "err.h"

static char buf[4096];

void
cp(char *f0, char *f1)
{
	int fd0, fd1;
	int len;

	if ((fd0 = open(f0, O_RDONLY)) < 0) {
		fatalf(ERR,
			"Tried to copy a file which does not exist (%s)!\n",
			f0);
	}

	fd1 = open(f1, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	len = read(fd0, buf, 4096);
	write(fd1, buf, len);

	close(fd0);
	close(fd1);
}

void
cat(char *path)
{
	int fd = open(path, O_RDONLY);
	int len = read(fd, buf, 4096);
	write(1, buf, len);
}
