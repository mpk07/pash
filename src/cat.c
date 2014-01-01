#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "../include/commons.h"

#define BUF_SIZE 4096

int my_cat(int argc, char *argv[])
{
	int ret = EXIT_FAILURE;
	int src, dst;

	ssize_t nread, nwritten;
	char buf[BUF_SIZE];

	if (argc >= 2) {
		if ((src = open(argv[1], O_RDONLY)) == -1) {
			error("open", argv[1]);
			goto open_fail;
		}
	} else {
		src = 0;
	}

	do { 
		if (argc > 1) {
			if ((nread = read(src, buf, BUF_SIZE)) == -1) {
				error("read", argv[1]);
				goto read_fail;
			}
		} else {
			if ((nread = read(src, buf, BUF_SIZE)) == -1) {
				error("read", argv[1]);
				goto read_fail;
			}
		}

		if ((nwritten = write(1, buf, nread)) == -1) {
			error("write", argv[2]);
			goto write_fail;
		}
	} while (nread > 0);

	ret = EXIT_SUCCESS;

write_fail:
read_fail:
	/*close(dst);*/
	close(src);
open_fail:
	return ret; 
}
