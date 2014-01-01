#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "../include/commons.h"

#define BUF_SIZE 4096

int my_cp(int argc, char *argv[]) 
{
    int ret = EXIT_FAILURE;
    int src, dst;

    ssize_t nread, nwritten;
    char buf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s source destination.\n", argv[0]);
        goto no_args;
    }

    if ((src = open(argv[1], O_RDONLY)) == -1) {
        error("open", argv[1]);
        goto open_fail;
    }

    if ((dst = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644)) == -1) {
		 /* O_EXCL is used along with O_CREAT, when a file 
		  * already exists, then fail, setting errno to EEXIST */
        error("open", argv[2]);
        goto open_dst_fail;

    }

    do {
        if ((nread = read(src, buf, BUF_SIZE)) == -1) {
            error("read", argv[1]);
            goto read_fail;
        }

        if ((nwritten = write(dst, buf, nread)) == -1) {
            error("write", argv[2]);
            goto write_fail;
        }

    } while (nread == BUF_SIZE); 

    ret = EXIT_SUCCESS;

write_fail:
read_fail:
    close(dst);
open_dst_fail:
    close(src);
open_fail:
no_args:
    return ret; 
}
