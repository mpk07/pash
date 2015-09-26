#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

static char *prev_dir;

char* get_whoami (void) {
   char *lgn;
   struct passwd *pw;

   if ((lgn = getlogin()) == NULL || (pw = getpwnam(lgn)) == NULL) {
      fprintf(stderr, "Get of user information failed.\n"); 
      exit(1);
   }

   if(pw) {
      return pw->pw_name;
   }
} 

int my_cd (int argc, char *argv[]) {
    int r, ret = EXIT_FAILURE;
    int i;
    char *temp, *home;

    /*printf("prev_dir=%s\n", prev_dir);*/

    if (argc < 2) {
       home = malloc(256*sizeof(char));
       // strcat expects all strings to be null-terminated
       home[0]='\0'; 
       strcat(home, "/home/");
       strcat(home, get_whoami());

       temp = get_current_dir_name();
       /*printf("temp=%s\n", temp);*/
       if((r = chdir((char*)home)) == -1) {
          perror(argv[i]);
       }
       prev_dir = temp;

       free(home);
       goto done;
    }

    if (strcmp(argv[1], "-") == 0) {
       if (prev_dir == NULL) {
          // if you want to invoke 'pwd',
          // you may, otherwise a no-op should
          // suffice
          ((void)0);
       } else {
          temp = get_current_dir_name();
          /*printf("temp=%s\n", temp);*/
          if((r = chdir(prev_dir)) == -1) {
             perror(argv[i]);
          }
          prev_dir = temp;
       }
    } else {
       temp = get_current_dir_name();
       /*printf("temp=%s\n", temp);*/
       if((r = chdir(argv[1])) == -1) {
          perror(argv[i]);
       }
       prev_dir = temp;
    }
done:
    ret = EXIT_SUCCESS;

fail:    
    return ret;
}

