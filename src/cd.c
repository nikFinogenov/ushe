#include "ush.h"

void cd(char* path) {
    // if (path == NULL) path = HOME;/
    // struct passwd *pw = getpwuid(getuid());
    // HOME = pw->pw_dir;
    // const char *homedir;

        // if ((homedir = getenv("HOME")) == NULL) {
            const char* homedir = getpwuid(getuid())->pw_dir;
        // }
    if (path == NULL) {path = malloc(strlen(homedir) + 1); mx_strcpy(path, homedir);}
    if (chdir(path) == 0) {

    } else {
        perror("chdir");
        return;
    }
}
