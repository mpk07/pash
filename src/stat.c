#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include "../include/commons.h"
#define BUFSIZE 4096

int my_stat(int argc, char **argv) {
/*int main(int argc, const char **argv) {*/
	int ret = EXIT_FAILURE;
	int i,r, fd=0;

	char lname[BUFSIZE];
	char buf[BUFSIZE];

	struct stat fileStat;
	struct tm *tm;

	if(argc < 2) {
		fprintf(stderr, "usage: %s <file1> ...\n", argv[0]);
		goto fail;
	}

	for (i=1; i<argc; i++) {

		/*if((fd=open(argv[1],O_RDONLY)) < -1) {*/
			/*perror("open");*/
			/*goto open_fail;*/
		/*}*/

		if(lstat(argv[1], &fileStat) < 0) {
			perror("stat");
			goto lstat_fail;
		}
		
		printf("File: %s\t",argv[1]);

		lname[fileStat.st_size] = '\0';

		printf("File Size: %d bytes\n", (int)fileStat.st_size);
		printf("#Blocks: %d\t", (int)fileStat.st_blocks);
		printf("File inode: %d\n", (unsigned int)fileStat.st_ino);
		printf("Number of Links: %d\n", (unsigned int)fileStat.st_nlink);

		switch (fileStat.st_mode & S_IFMT) {
			case S_IFBLK:  printf("File type: block device  (b");            break;
			case S_IFCHR:  printf("File type: character device  (c");        break;
			case S_IFDIR:  printf("File type: directory  (d");               break;
			case S_IFIFO:  printf("File type: FIFO/pipe  (p");               break;
			case S_IFLNK:  //have to read the link to display the file being pointed to 
								r = readlink(argv[1], lname, fileStat.st_size+1);
								if (r < 0) {
									perror("readlink");
									exit(EXIT_FAILURE);
								}

								if (r > fileStat.st_size) {
									fprintf(stderr, "symlink increased in size "
											"between lstat() and readlink()\n");
									exit(EXIT_FAILURE);
								}

								printf("'%s' --> '%s'\n", argv[1], lname);
								printf("File type: symlink  (l");
								break;
			case S_IFREG:  printf("File type: regular file  (-");            break;
			case S_IFSOCK: printf("File type: socket  (s");                  break;
			default:       printf("File type: unknown?\ (n");                break;
		}

		printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
		printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
		printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
		printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
		printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
		printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
		printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
		printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
		printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

		printf(")\n");
		printf("UID: %d\tGID: %d\n\n", (int)fileStat.st_uid, (int)fileStat.st_gid);
		
		/* convert time_t to broken-down time representation */
		tm = localtime(&fileStat.st_ctim);
		/* format time days.month.year hour:minute:seconds */
		strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", tm);
		printf("Last status change:      \t%s\n", buf);

		tm = localtime(&fileStat.st_ctim);
		strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", tm);
		printf("Last file access:        \t%s\n", buf);

		tm = localtime(&fileStat.st_ctim);
		strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", tm);
		printf("Last file modification: \t%s\n", buf);

	}

	ret = EXIT_SUCCESS;
lstat_fail:
readlink_fail:
	/*close(fd);*/
open_fail:
fail:
	return ret;
}
