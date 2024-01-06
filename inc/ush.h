#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <stdio.h>
#include "../libmx/inc/libmx.h"
#include <string.h>
#include <pwd.h>

void pwd(void);
void init(void);
void cd(char* path);
char* command_format(char* command);
void echo(char* line);


// extern char* HOME;
#endif
