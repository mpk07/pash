#include <sys/utsname.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

static int verbose_flag;

int my_ln(int argc, char*argv[]) {
	int ret=EXIT_FAILURE;

	int c, s, it=1;

	if (argc < 3) {
		fprintf(stderr, "usage: %s [options] <file1> <link>\n", argv[0]);
		goto fail;
	}

	while (it) {
		static struct option long_options[] =
		{
			/* These options set a flag. */
			{"verbose", no_argument,       &verbose_flag, 1},
			{"brief",   no_argument,       &verbose_flag, 0},
			/* These options don't set a flag. We distinguish them by their indices. */
			{"symbolic", no_argument,       0, 's'},
			{"backup",   no_argument,       0, 'b'},
			{"force",    required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "s:bf", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1 && it == 1) {
			if ((s = link(argv[1], argv[2])) == -1) {
				perror(argv[0]);
				goto fail;
			}
			goto success;
		} else if (it > 1 && c == -1) {
			break;
		}

		switch (c)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;

			case 's':
				if ((s = symlink(argv[2], argv[3])) == -1) {
					perror(argv[0]);
					goto fail;
				}
				break;

			case 'b':
				puts ("yet to implement\n");
				break;

			case 'f':
				printf ("option -f with value `%s'\n", optarg);
				printf ("yet to implement\n", optarg);
				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				/*abort ();*/ //how did this come?
				break;
		}
		it++;
	}

success:
	ret=EXIT_SUCCESS;
fail:
	return ret;
}
