#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <readline/readline.h> //good for interactive shells
#include <readline/history.h>
#include <wordexp.h>
#include <dlfcn.h>
#include <signal.h>

#include "../include/commons.h"

void signal_handler(int signo) {

   if (signo == SIGINT) {
      ((void)0); //no-op
   } else if (signo == SIGTSTP) {
      ((void)0); //no-op
   } else if (signo == SIGQUIT) {
      ((void)0); //no-op
   } else if (signo == SIGTERM) {
      ((void)0); //no-op
   } else {
      printf("Unexpected signal caught\n");
      exit(EXIT_FAILURE);
   }
   return;
}

void register_signal_handler (void) {
    /* handle SIGINT but only if it isn't ignored */
    if (signal(SIGINT, SIG_IGN) != SIG_IGN) {
       if (signal(SIGINT, signal_handler) == SIG_ERR) {  // handler call for ctrl+c
          // in a properly working machine, the code shouldn't come here
          fprintf(stderr, "can't catch SIGINT\n"); 
          exit(EXIT_FAILURE); 
       }
    }

    /* handle SIGTSTP but only if it isn't ignored */
    if (signal(SIGTSTP, SIG_IGN) != SIG_IGN) {
       if (signal(SIGTSTP, signal_handler) == SIG_ERR) { // handler call for ctrl+z
          // in a properly working machine, the code shouldn't come here
          fprintf(stderr, "can't catch SIGTSTP\n"); 
          exit(EXIT_FAILURE); 
       }
    }

    /* handle SIGQUIT but only if it isn't ignored */
    if (signal(SIGQUIT, SIG_IGN) != SIG_IGN) {
       if (signal(SIGQUIT, signal_handler) == SIG_ERR) { // handler call for ctrl+z
          // in a properly working machine, the code shouldn't come here
          fprintf(stderr, "can't catch SIGQUIT\n"); 
          exit(EXIT_FAILURE); 
       }
    }

    /* handle SIGTERM but only if it isn't ignored */
    /* not able to withstand kill -15 anyway. pash is getting killed */
    /*
     *if (signal(SIGTERM, SIG_IGN) != SIG_IGN) {
     *   if (signal(SIGTERM, signal_handler) == SIG_ERR) { // handler call for ctrl+z
     *      // in a properly working machine, the code shouldn't come here
     *      fprintf(stderr, "can't catch SIGTERM\n"); 
     *      exit(EXIT_FAILURE); 
     *   }
     *}
     */
   return;
}

int main(int argc, char **argv) {
    char *input;
    char libname[30], funcname[60], find_expr[100], modified_input[100];

    wordexp_t args; 
    //args.we_wordc will have number of arguments
    //args.we_wordv will have the arguments themselves
    
	 boolean in_usrbin, in_bin, in_mybin;
    int i =20;
    void* handle;

    int (*cmd)(int, char**);

    register_signal_handler();
    
	 while(1) {
		 //the param is a prompt string; readline does dynamic allocation
       input=readline("pash % ");
       /*input=readline(getenv("PS1")); */
       
       /*
        * From the man page - if readline sees an EOF, it returns NUll. This is
        * the way to handle ctrl-d
        */
       if (input == NULL) 
          goto end;

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

		 sprintf(find_expr, "find /bin/%s > /dev/null 2>&1", args.we_wordv[0]);/*{{{*/
		 in_bin = system(find_expr);
		 sprintf(find_expr, "find /usr/bin/%s > /dev/null 2>&1", args.we_wordv[0]);
		 in_usrbin = system(find_expr);
		 sprintf(find_expr, "find /home/mpk/bin/%s > /dev/null 2>&1", args.we_wordv[0]);
		 in_mybin = system(find_expr);/*}}}*/

		 if (!handle) {/*{{{*/
			 if (!in_bin) {
				 sprintf(modified_input, "/bin/%s", input);
				 system(modified_input);
				 continue;
			 } else if (!in_usrbin) {
				 sprintf(modified_input, "/usr/bin/%s", input);
				 system(modified_input);
				 continue;
			 } else if (!in_mybin) {
				 sprintf(modified_input, "/home/mpk/bin/%s", input);
				 system(modified_input);
				 continue;
			 } else {
				 printf("%d: Error: %s\n", __LINE__, dlerror());
				 continue;
			 }
		 }/*}}}*/

		 sprintf(funcname, "my_%s",args.we_wordv[0]);
		 cmd = dlsym(handle, funcname);
       /*
        * funcname = my_chdir/my_whoami
        * if the .so has the function as create_dir(), then the second param should be
        * "create_dir" so that create_dir is looked up
        * cmd will get the function pointer by looking up a symbol table and 
        * get the address associated with the function name
        */

		 if(!cmd) {
			 printf("Error: %s\n", dlerror());
			 continue;
		 }
		 cmd(args.we_wordc, args.we_wordv);

       dlclose(handle);
		 add_history(input); //add_history maintains pointers for the previously entered i/p to be accessible
       free(input); //shouldn't free(input) if this is called. 
	 }
end:
    return 0; //note that this value can max be 255
}
