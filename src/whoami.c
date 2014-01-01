#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int my_whoami(int argc, char **argv) {
	int ret = EXIT_FAILURE;
	char *lgn;
	struct passwd *pw;

	if ((lgn = getlogin()) == NULL || (pw = getpwnam(lgn)) == NULL) {
		fprintf(stderr, "Get of user information failed.\n"); 
		exit(1);
	}
	
	if(pw) {
		printf("%s\n", pw->pw_name);
	}

	ret = EXIT_SUCCESS;
	return ret;
}
