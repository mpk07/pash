#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "../include/commons.h"

#define BUF_SIZE 4096

int my_yes(int argc, char *argv[])
{
    int ret = EXIT_FAILURE;
	 int acount;

    ssize_t nread, nwritten=0;
    char buf[BUF_SIZE];

	 if (argc == 1) {
		 while (nwritten != -1) {
			 nwritten = write(1, "y\n", 3); 
		 }
		 perror(argv[0]);
		 goto write_fail;
	 } else {
		 do {
			 //preparing buf
			 acount = 0;
			 while(acount+1 < argc) {
				 strcat(buf, argv[acount+1]);
				 if (acount != argc) {
					 strcat(buf, " ");
				 }
				 acount++;
			 }
			 strcat(buf, "\n");

			 if ((nwritten = write(1, buf, (int)(strlen(buf))+1)) == -1) {
				 error("write", argv[2]);
				 goto write_fail;
			 }

		 } while (1);
	 }

    ret = EXIT_SUCCESS;

write_fail:
    return ret; 
}

