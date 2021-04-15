#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "tokenizer.h"

#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
  char** args = malloc((MAX_LINE/2 + 1)* sizeof *args); /* command line arguments */
  int should_run = 1; /* flag to determine when to exit program */

  // previous run command
  char previous[MAX_LINE/2 + 1];
  previous[0] = '\0';

  while (should_run) {
    printf("osh>");
    fflush(stdout);

    // read user input and put in char array
    char cbuf[MAX_LINE/2 + 1];

    // clear cbuf
    memset(cbuf, '\0', sizeof(cbuf));

    fgets(cbuf, MAX_LINE/2 + 1, stdin);
    cbuf[strlen(cbuf) - 1] = '\0';

    // history feature loads previous command
    if (cbuf[0] == '!' && cbuf[1] == '!')
    {
      if (previous[0] == '\0')
      {
        printf("No commands in history\n");
      }
      strcpy(cbuf, previous);
    }
    else
    {
      // deep copy cbuf into 'previous'
      memset(previous, '\0', sizeof(previous));
      strcpy(previous, cbuf);
    }

    // tokenize user input
    tokenize(cbuf, args);
    // fork new process
    int pid = fork();

    // fork failed
    if (pid < 0)
    {
      printf("Error: failed to fork!");
    }
    // in child process
    else if (pid == 0)
    {
      // check for '>' output redirection
      int x = 0;
      int inputLocation = -1;
      int outputLocation = -1;

      while (args[x] != NULL)
      {
          printf("X is no %d\n", x);
          printf("Checking args:%s\n", args[x]);

        if (strcmp(">", args[x]) == 0)
        {
            printf("Found > at %d\n", x);
          outputLocation = x;
          // args[x] = '\0'; 
        }
        
        if (strcmp("<", args[x]) == 0)
        {
            printf("Found < at %d\n", x);
          inputLocation = x;
          
        }

        x++;
        
      }

      if (outputLocation != -1)
      {
          printf("outputlocation: %s\n", args[outputLocation + 1]);
        int file_desc1 = creat(args[outputLocation + 1], 0640);
        args[outputLocation] = NULL;       
        close(1);
        // 1 is stdout
        dup(file_desc1);
        close(file_desc1);
      }

      if (inputLocation != -1)
      {
          printf("inputlocation: %s\n", args[inputLocation + 1]);
        int file_desc2 = open(args[inputLocation + 1], O_RDWR);
        args[inputLocation] = NULL;
        close(0);
        dup(file_desc2);
        close(file_desc2);
      }

      // execute the command
      execvp(args[0], args);
    }
    // in parent process
    else
    {
      wait();
    }

    /**
    * After reading user input, the steps are:
    * (1) fork a child process using fork()
    * (2) the child process will invoke execvp()
    * (3) parent will invoke wait() unless command included &
    */
  }
  return 0;
}