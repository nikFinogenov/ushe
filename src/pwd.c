#include "ush.h"

void pwd(char* command) {
    char* res = NULL;
    char* str_flags = parse_flags(command, &res);
    t_pwd_flags_s flags;
    init_pwd_flags(&flags, str_flags);
    mx_strcpy(command, res);

    printf("%s\n", PWD);
}
