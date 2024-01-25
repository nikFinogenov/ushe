#include "ush.h"

void cd(char* command) {
    char* path = mx_strsplit(command, ' ')[1];
    if(mx_get_length(mx_strsplit(command, ' ')) <= 2){
        if (path == NULL) {
            path = malloc(strlen(HOME) + 1); 
            mx_strcpy(path, HOME);
        }
        if (chdir(path) == 0) {
            PREVPWD = PWD;
            setenv("OLDPWD", PREVPWD, 1);
            PWD = getcwd(NULL, 1024);
            setenv("PWD", PWD, 1);
        } else {
            perror("chdir");
            return;
        }
    }
    else{
        mx_printerr("ush: cd: string not in pwd: ");
        mx_printerr(path);
        mx_printerr("\n");
    }
}
