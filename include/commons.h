//some common #defines and some common header file inclusions shall be listed here

#define error(x, y)  fprintf(stderr, "%s: error while %s on %s: %s\n", argv[0], x, y, strerror(errno))
#define SIZE_OF_PATH 2040 // 256 bytes = 8x255
#define FILENAME_LENGTH 64 // 64 bytes

typedef unsigned short int boolean;
