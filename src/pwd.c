#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int my_pwd(int argc, char *argv[])
{
    int r, ret = EXIT_FAILURE;
    int i;

    if (argc > 1) {
        fprintf(stderr, "usage: %s<CR>\n", argv[0]);
        goto fail;
    }
    printf("%s\n", get_current_dir_name());
fail:    
    return ret;
}
