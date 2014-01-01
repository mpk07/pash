#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int my_date (int argc, char **argv) {
	struct timeval tv;
	struct tm* ptm;
	/* from time.h
	 *struct tm {
	 *   int tm_sec;         [> seconds <]
	 *   int tm_min;         [> minutes <]
	 *   int tm_hour;        [> hours <]
	 *   int tm_mday;        [> day of the month <]
	 *   int tm_mon;         [> month <]
	 *   int tm_year;        [> year <] a number to which 1900 should be added
	 *   int tm_wday;        [> day of the week <]
	 *   int tm_yday;        [> day in the year <]
	 *   int tm_isdst;       [> daylight saving time <]
	 *};
	 */
																																																			
	char time_string[40];
	long milliseconds;

	/* Obtain the time of day, and convert it to a tm struct. */
	gettimeofday (&tv, NULL);
	ptm = localtime (&tv.tv_sec);
	/* Format the date and time, down to a single second. 
	 * tm_year field prints 113 for 2013. hence, this call.
	 * hour:min:sec prints fine */
	strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
	milliseconds = tv.tv_usec / 1000;
	/*to print milliseconds too, use the below printf */
	/*printf ("%s.%03ld\n", time_string, milliseconds);*/
	printf ("%s\n", time_string);
	return 0;
}
