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

// #define DEFAULT_COLOR "\033[0m"
// #define BLUE_COLOR "\033[34m"
// #define GREEN_COLOR "\033[32m"
// #define MAGENTA_COLOR "\033[35m"
// #define YELLOW_COLOR "\033[33m"
// #define RED_COLOR "\033[31m"

typedef struct t_env_flags_s{
    bool i; //Execute the utility with only those environment variables specified by name=value options.  The environment inherited by env is ignored completely.
    bool P; //altpath | Search the set of directories as specified by altpath to locate the specified utility program, instead of using the value of the PATH environment variable.
    bool u; //name | If the environment variable name is in the environment, then remove it before processing the remaining options.  This is similar to the unset command in sh(1).  The value for name must not include the ‘=’ character. 
} t_env_flags_s; 

typedef struct t_cd_flags_s{
    bool s; //DONE Flag for option '1': List one entry per line.
    bool P; //DONE Flag for option '1': List one entry per line.
    bool hyphen; //DONE Flag for option '1': List one entry per line.
} t_cd_flags_s; 

typedef struct t_pwd_flags_s{
    bool L; //Display the logical current working directory.
    bool P; //Display the physical current working directory (all symbolic links resolved).
} t_pwd_flags_s;

typedef struct t_which_flags_s{
    bool a; //List all instances of executables found (instead of just the first one of each).
    bool s; //No output, just return 0 if all of the executables are found, or 1 if some were not found.
} t_which_flags_s;

typedef struct t_echo_flags_s{
    bool E; //xyi ego znaet
    bool e; //xyi ego znaet
    bool n; //Do not print the trailing newline character.
} t_echo_flags_s;



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
