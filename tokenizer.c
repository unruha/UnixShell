#include "tokenizer.h"

// tokenize user input into individual arguments
void tokenize(char* cbuf[], char* args[])
{
    // tokenize user input
    char* token;
    token = strtok(cbuf, " ");

    // add each token to arguments
    int i = 0;
    while (token != NULL)
    {
        printf("tokenizer:%s\n", token);
      args[i] = token;
      token = strtok(NULL, " ");
      ++i;
    }
    args[i] = NULL;
}