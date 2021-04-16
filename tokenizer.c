#include "tokenizer.h"

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