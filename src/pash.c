#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <readline/readline.h> //good for interactive shells
#include <readline/history.h>
#include <wordexp.h>
#include <dlfcn.h>

#include "../include/commons.h"


int main(int argc, char **argv) {
    char *input;
    char libname[30], funcname[60], find_expr[100], modified_input[100];

    wordexp_t args; 
    //args.we_wordc will have number of arguments
    //args.we_wordv will have the arguments themselves
    
	 boolean in_usrbin, in_bin;
    int i =20;
    void* handle;

    int (*cmd)(int, char**);
    
	 while(1) {
		 //the param is a prompt string; readline does dynamic allocation
		 input=readline("pash % ");
		 /*input=readline(getenv("PS1")); */
		 if (strcmp(input, "\0") == 0) {
			 continue;
		 } else if (strcmp(input, "exit") == 0) {
			 break;
		 }

		 wordexp(input, &args, 0);
		 sprintf(libname,"/home/mpk/coding/shell/lib/lib%s.so",args.we_wordv[0]);
		 handle = dlopen(libname, RTLD_LAZY);
		 /*
		  * handle = dlopen("./libmkdir.so", RTLD_LAZY); -> lazy loading of a library
		  * when RTD_LAZY is used, the mmap happens only when the call happens 
		  * during execution (on page fault)
		  *
		  * the 0th arg is the shared library that is created in this dir.
		  * it does a mkdir but the function will be with the name main.
		  * this function also checks if the file being searched for (wordv[0]) is an .so, 
		  * if it's not then the dlopen will fail
		  */

		 /* Loop through the dirs in the $PATH and find the executable */

		 sprintf(find_expr, "find /bin/%s > /dev/null 2>&1", args.we_wordv[0]);
		 in_bin = system(find_expr);
		 sprintf(find_expr, "find /usr/bin/%s > /dev/null 2>&1", args.we_wordv[0]);
		 in_usrbin = system(find_expr);

		 if (!handle) {
			 if (!in_bin) {
				 sprintf(modified_input, "/bin/%s", input);
				 system(modified_input);
				 continue;
			 } else if (!in_usrbin) {
				 sprintf(modified_input, "/usr/bin/%s", input);
				 system(modified_input);
				 continue;
			 } else {
				 printf("%d: Error: %s\n", __LINE__, dlerror());
				 continue;
			 }
		 }

		 /*printf("%s\n", funcname);*/
		 sprintf(funcname, "my_%s\0",args.we_wordv[0]);
		 cmd = dlsym(handle, funcname); 
		 //this should be the function name present in the .so file 
		 //if the .so has the function as create_dir(), then the second param should be
		 //"create_dir" so that create_dir is looked up
		 //cmd will get the function pointer by looking up a symbol table and 
		 //get the address associated with the function name

		 if(!cmd) {
			 printf("Error: %s\n", dlerror());
			 continue;
		 }
		 cmd(args.we_wordc, args.we_wordv);

		 dlclose(handle);
		 add_history(input); 
		 //shouldn't free(input) if this is called. 
		 //add_history maintains pointers for the previously entered i/p to be accessible
	 }
    return 0; //note that this value can max be 255
}
