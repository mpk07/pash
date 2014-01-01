/* Following functionalities have been considered:
 * - if the file is not presently there, it will be created
 * - if the file is already there, then timestamp will be changed [not implemented]
 * - will take care of multiple parameters
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <readline/readline.h> 

#include "../include/commons.h"

int my_touch (int argc, char **argv) {
	int ret = EXIT_FAILURE, ch_ret;
	int i, src, dummy;
	int default_permissions =
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	struct timespec *times = NULL;

	if (argc < 2) {
		fprintf(stderr, "usage: %s [options] <file1> ...\n", argv[0]);
		goto fail;
	}
	
	for (i=1; i < argc; i++) {
		if ((src = open(argv[1], O_WRONLY | O_CREAT | O_EXCL)) == -1) {
#ifdef DEBUG
			printf("file already exists:%d, %d.\n", errno, EEXIST);
#endif
			if ((src = open(argv[1], O_RDONLY)) == -1) {
				error("open", argv[i]);
				goto open_fail;
			}
		}
#ifdef DEBUG
		printf("opened file.\n");
#endif

		/* file created; set default permissions 
		 * in coreutils code, this is done using fd_reopen*/
		if (errno == EEXIST) {
			dummy = futimens(src, times);
			goto already_exists;
		} else {
#ifdef DEBUG
			printf("file created; setting permissions.\n");
#endif
			if ((ch_ret = fchmod(src, (mode_t) default_permissions)) != 0) {
				error("chmod", argv[i]);
				goto chmod_fail;
			}
		}

already_exists:
chmod_fail:
#ifdef DEBUG
		printf("closing file.\n");
#endif
		close(src);
	}

	ret = EXIT_SUCCESS;
open_fail:
fail:
	return ret;
}
