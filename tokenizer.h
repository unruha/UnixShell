#ifndef tokenizer
#define tokenizer

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

#endif