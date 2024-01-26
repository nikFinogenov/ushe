#include "ush.h"

void cd(char* command) {
    char* res = NULL;
    char* str_flags = parse_flags(command, &res);
    t_cd_flags_s flags;
    init_cd_flags(&flags, str_flags);
    mx_strcpy(command, res);

    char* path = mx_strsplit(command, ' ')[1];
    if(mx_get_length(mx_strsplit(command, ' ')) <= 2){
        if (path == NULL) {
            path = HOME;
        }
        if(mx_strcmp(path, "-") == 0) {
            path = PREVPWD;
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
