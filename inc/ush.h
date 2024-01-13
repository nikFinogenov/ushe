#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <stdio.h>
#include "../libmx/inc/libmx.h"
#include <string.h>
#include <pwd.h>

#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/acl.h>
#include <sys/ioctl.h>
#include <grp.h>
#include <errno.h>

#define DEFAULT_COLOR "\033[0m"
#define BLUE_COLOR "\033[34m"
#define GREEN_COLOR "\033[32m"
#define MAGENTA_COLOR "\033[35m"
#define YELLOW_COLOR "\033[33m"
#define RED_COLOR "\033[31m"

typedef struct s_flags_s{
    bool one; //DONE Flag for option '1': List one entry per line.
    bool C;   //DONE Flag for option 'C': Force multi-column output.
    bool m;   //DONE Flag for option 'm': Comma separated output
    bool l;   //DONE Flag for option 'l': Long format, showing detailed file information.
    bool c;   //DONE Flag for option 'c': Sort by change time, showing the newest first.
    bool u;   //DONE Flag for option 'u': Sort by access time, showing the newest first.
    bool U;   //DONE Flag for option 'U': Use time when file was created for sorting or printing
    bool t;   //DONE Flag for option 't': Sort by modification time, showing the newest first.
    bool S;   //DONE Flag for option 'S': Sort by file size, showing the largest first.
    bool f;   //DONE Flag for option 'f': Disable sorting, and list entries in the order they appear.
    bool A;   //DONE?Flag for option 'A': Lists all entries except for '.' and '..'.
    bool a;   //DONE Flag for option 'a': Usually stands for "all", showing hidden files.       
    bool R;   //DONE Flag for option 'R': Recursively list subdirectories.
    bool G;   //DONE Flag for option 'G': Enable colored output.
    bool F;   //DONE Flag for option 'F': Append a character to entries to indicate their type.
    bool p;   //DONE Flag for option 'p': Add a trailing '/' to directory names.
    bool r;   //DONE Flag for option 'r': Reverse the order of listing.
    bool h;   //DONE Flag for option 'h': Human-readable file sizes (e.g., in KB, MB).
    bool e;   //DONE?Flag for option 'e': Display the escape sequences for control characters.
    bool T;   //DONE Flag for option 'T': Display full time information.
    bool at;  //DONE Flag for option '@': Extended attribute keys.
    bool g;   //DONE Flag for option 'g': Like 'l', but excludes the owner.
    bool o;   //DONE Flag for option 'o': Like 'l', but excludes the group.
    bool x;   //DONE?Flag for option 'x': List entries by lines, rather than columns.
    bool n;   //DONE Flag for option 'n': List numeric user and group IDs instead of names.
    bool i;   //DONE!Flag for option 'i': Show the file's inode number.
} t_flags_s;

typedef struct {
    unsigned long inode;
    char *name;
    char *path;
    char type;
    char permissions[11];
    int nlinks;
    char owner[256];
    char group[256];
    long size;
    char human_size[256];
    char date_time[20];
    struct timespec cmptime;
    char symlink[1024];
    char **xattr_keys;
} t_file_entry_s;

typedef struct {
    int max_nlinks_len;
    int max_username_len;
    int max_groupname_len;
    int max_size_len;
    int max_h_size_len;
} t_max_sizes_s;



void pwd(void);
void init(void);
void cd(char* path);
char* command_format(char* command);
void echo(char* line);
void my_exit(void);
void ls(int argc, char **argv);


// extern char* HOME;
#endif
