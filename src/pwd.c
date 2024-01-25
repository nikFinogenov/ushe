#include "ush.h"

void pwd(char* command) {
    char* str_flags = parse_flags(command);
    t_pwd_flags_s flags;
    init_pwd_flags(&flags, str_flags);

    printf("%s\n", PWD);
}
