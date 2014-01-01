/* 
 * mv should achieve the following
 * 	- rename a single file
 * 	- move first n-1 parameters to the last parameter
 * 	- if renaming a file with an existing file name, inode must be updated 
 * 	  and the resultant file should over-write the destination file.
 * 	  example: a folder contains foo & bar.
 * 	  			  mv foo bar should replace bar with foo but with the name of bar
 * PSEUDOCODE:
 * 	+ if the last argument = dir:
 * 		for all arguments
 * 			rename (arg, dir/arg)
 * 	+ else 
 * 		if last arguemnt != dir:
 * 			if argc == 2
 * 				rename (arg1, arg2)
 * 			else
 * 				perror: last arg is not a directory
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "../include/commons.h"

// modify this value to run this library module in debug-friendly mode
// also re-compile this library by using ./build.sh mv
int debug=0; 

void retrieve_filename_only(char **filename_only, char path[]) {

	char *pch = NULL;
	int ret_print;

	pch = strtok (path,"/");
	while (pch != NULL)
	{
		*filename_only = pch;
		pch = strtok (NULL, "/");
	}
}

int my_mv(int argc, char *argv[]) {
/*int main(int argc, char *argv[]) {*/
	int ret = EXIT_FAILURE;
	int is_dir=0;
	struct stat last_arg_stat;
	char newpath[SIZE_OF_PATH], *filename_only;
	char temp[SIZE_OF_PATH];
	int i;

	if (argc < 3) {
	  fprintf(stderr, "usage: %s source destination.\n", argv[0]);
	  goto no_args;
	}

	if(stat(argv[argc-1], &last_arg_stat) < 0) {
		//case where no file exists therefore cannot be taken
		//as a directory
		is_dir = 0;
	} else {
		is_dir = S_ISDIR(last_arg_stat.st_mode);
	}

	if (is_dir) {
		// case where the last argument is a directory
		// there are two cases here.
		// 	1. relative paths - no problem.
		// 	2. absolute paths - the last part of the absolute path which is the filename
		// 		has to be found and then call the sprintf
		for (i=1; i < argc-1; i++) {

			// get the filename alone and get rid of the rest of the path
			sprintf(temp, "%s", argv[i]);
			retrieve_filename_only(&filename_only, temp);
			if (debug) printf("filename only: %s\n", filename_only);

			// create new path
			sprintf(newpath, "%s/%s", argv[argc-1], filename_only);
			if (debug) printf("newpath=%s\n", newpath);

			if (rename(argv[i], newpath)) {
				perror(argv[i]);
			}
		}
	} else { // the case where the last argument is not a directory
		if (argc >= 4) {
			fprintf(stderr, "%s is not a directory\n", argv[argc-1]);
		} else {
			// rename call for that one file
			if(rename(argv[1], argv[2])) {
				perror(argv[0]);
			}
		}
	}

	ret = EXIT_SUCCESS;
no_args:
	return ret;
}
