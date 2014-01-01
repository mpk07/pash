#include <sys/utsname.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

int my_uname(int argc, char *argv[]) {
	struct utsname buf;
	int opt;

	if(uname(&buf) == -1) {
		return EXIT_FAILURE;
	}
	if (argc == 1) {
		printf("%s\n", buf.sysname);
	} else if(argc>=1) {
		while ((opt = getopt(argc, argv, "anvsr")) != -1) {
			// options to be implemented with getopt_long
			switch (opt) {
				case 'a':
					printf("%s, %s, %s, ", buf.sysname, buf.nodename, buf.release); 
					printf("%s\n", buf.version); 
					break;
				case 'v':
					printf("%s\n", buf.version); 
					break;
				case 'n':
					printf("%s\n", buf.nodename); 
					break;
				case 's':
					printf("%s\n", buf.sysname); 
					break;
				case 'r':
					printf("%s\n", buf.release); 
					break;
				case 'p':
					printf("not implemented\n", buf.release); 
					break;
				default:
					exit(EXIT_FAILURE);
			}
		}
	}
	return EXIT_SUCCESS;
}
