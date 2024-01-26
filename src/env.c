#include "ush.h"

void env(char* command) {
    char* res = NULL;
    char* str_flags = parse_flags(command, &res);
    t_env_flags_s flags;
    init_env_flags(&flags, str_flags);
    mx_strcpy(command, res);
    
    printf("env poka nihuya ne delaet\n");
}
