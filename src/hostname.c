#include <sys/utsname.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

int my_hostname(int argc, char *argv[]) {
	struct utsname buf;
	int opt;

	if(uname(&buf) == -1) {
		return EXIT_FAILURE;
	}
	if (argc == 1) 
		printf("%s\n", buf.nodename);
	//options to be implemented
	return EXIT_SUCCESS;
}

