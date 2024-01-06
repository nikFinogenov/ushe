#include "ush.h"

void pwd(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        mx_printstr(cwd);
        mx_printchar('\n');
    } else {
        perror("getcwd");
        return;
    }
}
