#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int my_chdir(int argc, char *argv[])
{
    int r, ret = EXIT_FAILURE;
    int i;

    if (argc < 2) {
        fprintf(stderr, "usage: %s directory[...]\n", argv[0]);
        goto fail;
    }

    if((r = chdir("/tmp")) == -1) {
        perror(argv[i]);
    }
    ret += r;

fail:    
    return ret;
}
