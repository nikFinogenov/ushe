#include "ush.h"

void pwd(char* command) {
    char* res = NULL;
    char* str_flags = parse_flags(command, &res);
    t_pwd_flags_s flags;
    int status = init_pwd_flags(&flags, str_flags);
    mx_strcpy(command, res);
    if(mx_get_length(mx_strsplit(command, ' ')) == 1) {
        if(status == 0) {
            if(flags.P) {
                printf("%s\n", getcwd(NULL, 1024));
            }
            else printf("%s\n", PWD);
        }
    }
    else {
        mx_printerr("ush: pwd: too many arguments\n");
    }
}
