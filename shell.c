#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 80 /* The maximum length command */
void tokenize(char* cbuf, char* args[]);
void tokenizeFull(char* command, char* args[]);

// tokenizes an individual command
void tokenize(char* cbuf, char* args[])
{
    // tokenize user input
    char* token;
    token = strtok(cbuf, " ");

    // add each token to arguments
    int i = 0;
    while (token != NULL)
    {
      // printf("tokenizer:%s\n", token);
      args[i] = token;
      token = strtok(NULL, " ");
      ++i;
    }
    args[i] = NULL;
}

// tokenizes the original command list
void tokenizeFull(char* command, char* args[])
{
  if (strstr(command, "&") == NULL)
  {
    args[0] = command;
    return;
  }
  char* token;
  token = strtok(command, "&");

  // add each token to arguments
  int i = 0;
  while (token != NULL)
  {
    // printf("tokenizer full: %s\n", token);
    args[i] = token;
    token = strtok(NULL, "&");
    ++i;
  }
  args[i] = NULL;
}

int main(void)
{
  char** args = malloc((MAX_LINE/2 + 1)* sizeof *args); /* command line arguments */
  int should_run = 1; /* flag to determine when to exit program */
  
  // read user input and put in char array
  char* cbuf = malloc(sizeof(char) * 128);
  char** flags = malloc((MAX_LINE/2 + 1)* sizeof *flags);

  // previous run command
  char previous[MAX_LINE/2 + 1];
  previous[0] = '\0';

  while (should_run) {
    printf("osh>");
    fflush(stdout);

    // clear cbuf and args
    memset(cbuf, '\0', sizeof(cbuf));
    memset(args, '\0', sizeof(args));

    fgets(cbuf, MAX_LINE/2 + 1, stdin);
    cbuf[strlen(cbuf) - 1] = '\0';

    // scan for '&'
    char* shouldWait = strstr(cbuf, "&");
    char* colon;
    colon = strstr(cbuf, ";");
    if (colon != NULL)
    {
      strncpy(colon, "", 1);
    }

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
    tokenizeFull(cbuf, args);

    int y = 0;
    while (args[y] != NULL)
    {


      memset(flags, '\0', sizeof(flags));

      // tokenize user input
      tokenize(args[y], flags);
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

        while (flags[x] != NULL)
        {
            // printf("X is no %d\n", x);
            // printf("Checking args:%s\n", flags[x]);

          if (strcmp(">", flags[x]) == 0)
          {
            // printf("Found > at %d\n", x);
            outputLocation = x;
            // args[x] = '\0'; 
          }
          
          if (strcmp("<", flags[x]) == 0)
          {
            // printf("Found < at %d\n", x);
            inputLocation = x;
            
          }

          x++;
          
        }

        if (outputLocation != -1)
        {
          // printf("outputlocation: %s\n", flags[outputLocation + 1]);
          int file_desc1 = creat(flags[outputLocation + 1], 0640);
          flags[outputLocation] = NULL;       
          close(1);
          // 1 is stdout
          dup(file_desc1);
          close(file_desc1);
        }

        if (inputLocation != -1)
        {
          // printf("inputlocation: %s\n", flags[inputLocation + 1]);
          int file_desc2 = open(flags[inputLocation + 1], O_RDWR);
          flags[inputLocation] = NULL;
          close(0);
          dup(file_desc2);
          close(file_desc2);
        }

        // execute the command
        execvp(flags[0], flags);
        exit(0);
      }
      // in parent process
      else
      {
        if (shouldWait == NULL)
        {
          wait(NULL);
        }
        
        y++;
      }
      
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