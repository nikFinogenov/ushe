#include "ush.h"

void cd(char* path) {
    if (path == NULL) {
        path = malloc(strlen(HOME) + 1); 
        mx_strcpy(path, HOME);
    }
    if (chdir(path) == 0) {
        PREVPWD = PWD;
        PWD = getcwd(NULL, 1024);
    } else {
        perror("chdir");
        return;
    }
}
