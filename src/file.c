#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#include "../include/commons.h"

/*int my_file(int argc, char **argv) {*/
int main(int argc, char **argv) {
	int ret = EXIT_FAILURE;
	int i, fd=0;

	struct stat fileStat;

	if(argc <= 1) {
		fprintf(stderr, "usage: %s <file1> ...\n", argv[0]);
		goto fail;
	}

	for (i=1; i<argc; i++) {

		if((fd=open(argv[1],O_RDONLY)) < -1) {
			perror("open");
			goto open_fail;
		}

		if(fstat(fd,&fileStat) < 0) {
			perror("open");
			goto fstat_fail;
		}
	}

	ret=EXIT_SUCCESS;
fstat_fail:
	close(fd);
open_fail:
fail:
	return ret;
}
