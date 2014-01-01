#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int my_rm(int argc, char **argv) {
/*int main(int argc, char **argv) {*/
	int ret = EXIT_FAILURE, r, i;
		
	if (argc < 2) {
		fprintf(stderr, "usage: %s <file1> ...\n", argv[0]);
		goto fail;
	}

	for (i=1; i<argc; i++) {
		if((r = unlink(argv[i])) == -1) {
			perror(argv[i]);
		}
		/*ret += r;*/
	}
	
	ret = EXIT_SUCCESS;
fail:
	return ret;
}
