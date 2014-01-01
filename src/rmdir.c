#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int my_rmdir(int argc, char *argv[])
{
    int r, ret = EXIT_FAILURE;
    int i;

    if (argc < 2) {
        fprintf(stderr, "usage: %s directory[...]\n", argv[0]);
        goto fail;
    }

    for (i=1; i<argc; i++) {
		  /* the rmdir in the following line is the system_call
			* with the same name as the command
			*/
        if((r = rmdir(argv[i])) == -1) {
            perror(argv[i]);
        }
        ret += r;
    }

fail:    
    return ret;
}
