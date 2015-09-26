#include <sys/stat.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

int my_readlink(int argc, char *argv[])
{
	struct stat sb;
	char *linkname;
	ssize_t r;
	int ret = EXIT_FAILURE;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		goto args_fail;
	}

	if (lstat(argv[1], &sb) == -1) {
		perror("lstat");
		goto lstat_fail;
	}

	linkname = malloc(sb.st_size + 1);
	if (linkname == NULL) {
		fprintf(stderr, "insufficient memory\n");
		goto fail;
	}

	r = readlink(argv[1], linkname, sb.st_size + 1);

	if (r < 0) {
		//not a link; just exit
		goto success;
	}

	if (r > sb.st_size) {
		fprintf(stderr, "symlink increased in size "
				"between lstat() and readlink()\n");
		goto fail;
	}

	linkname[sb.st_size] = '\0';

	printf("'%s'\n", linkname);
success:
	ret = EXIT_SUCCESS;

args_fail:
lstat_fail:
fail:
   free(sb);
	return ret;
}

