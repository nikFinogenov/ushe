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
#include <sys/stat.h>

// #include <dirent.h>
// #include <time.h>
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
    bool s; //DONE xyi ego znaet|xyi ego znaet|xyi ego znaet|xyi ego znaet|xyi ego znaet|xyi ego znaet|xyi ego znaet|
    bool P; //DONE use the physical directory structure without following symbolic links: resolve symbolic links in DIR before processing instances of `..'
    bool L; //DONE taze ebateka chto s pwd
} t_cd_flags_s; 

typedef struct t_pwd_flags_s{
    bool L; //DONE Display the logical current working directory.
    bool P; //DONE Display the physical current working directory (all symbolic links resolved).
} t_pwd_flags_s;

typedef struct t_which_flags_s{
    bool a; //List all instances of executables found (instead of just the first one of each).
    bool s; //No output, just return 0 if all of the executables are found, or 1 if some were not found.
} t_which_flags_s;

typedef struct t_echo_flags_s{
    bool E; //disable interpretation of backslash escapes (default)
    bool e; //enable interpretation of backslash escapes
    bool n; //Do not print the trailing newline character.
} t_echo_flags_s;



extern char **environ;
void pwd(char* command);
void init(void);
void cd(char* path);
char* command_format(char* command);
void echo(char* line);
void my_exit(void);
int export(char* arguments);
int unset(char* arguments);
int which(char* arguments);
void env(char* command);

char* replace_tilda(char* argument, int *flag);
char* replace_tilda_backwards(char* argument, int flag);

char* parse_flags(char* command, char** result);
int init_cd_flags(t_cd_flags_s *flags, char* str_flags);
void init_env_flags(t_env_flags_s *flags, char* str_flags);
void init_which_flags(t_which_flags_s *flags, char* str_flags);
void init_echo_flags(t_echo_flags_s *flags, char* str_flags);
int init_pwd_flags(t_pwd_flags_s *flags, char* str_flags);
bool check_buildin(char* command);
bool operation_parametr_dollar_anal_variable_equals_peremenaya_detect_cum(char* line_command);
bool only_dollar(char* line_command);
// void create_var(char*** vars, char* line_command);
void output_var(char* var_name);

char* HOME;
char* PWD;
char* PREVPWD;
char* PATH;

#endif
