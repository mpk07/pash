//create a file and include this function in that file with the name 
//"main" because that is what is being searched for
//then compile that file as a shared object
//$ cc thatfile.c -o mkdir.so -lreadline -l<onemorename> -ldl

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int my_mkdir(int argc, char *argv[])
{
    int r, ret = EXIT_FAILURE;
    int i;

    if (argc < 2) {
        fprintf(stderr, "usage: %s directory[...]\n", argv[0]);
        goto fail;
    }

    for (i=1; i<argc; i++) {
        if((r = mkdir(argv[i], 0644)) == -1) {
            perror(argv[i]);
        }
        ret += r;
    }

fail:    
    return ret;
}

