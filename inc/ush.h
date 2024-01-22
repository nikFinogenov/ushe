#ifndef USH_H
#define USH_H

#define _GNU_SOURCE
#define __DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L //for setenv() and unsetenv()
#define _XOPEN_SOURCE 500 // for realpath() on Linux

#include <unistd.h>
#include <stdio.h>
#include "../libmx/inc/libmx.h"
#include <string.h>
#include <pwd.h>

// #include <dirent.h>
// #include <time.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/xattr.h>
// #include <sys/acl.h>
// #include <sys/ioctl.h>
// #include <grp.h>
// #include <errno.h>

#define DEFAULT_COLOR "\033[0m"
#define BLUE_COLOR "\033[34m"
#define GREEN_COLOR "\033[32m"
#define MAGENTA_COLOR "\033[35m"
#define YELLOW_COLOR "\033[33m"
#define RED_COLOR "\033[31m"

extern char **environ;
void pwd(void);
void init(void);
void cd(char* path);
char* command_format(char* command);
void echo(char* line);
void my_exit(void);
int export(char* arguments);
int unset(char* arguments);

void ls(int argc, char **argv);
char* replace_tilda(char* argument, int *flag);
char* replace_tilda_backwards(char* argument, int flag);

char* replaceSubstring(char* original, const char* substring, const char* replacement);

char* HOME;
char* PWD;
char* PREVPWD;

#endif
