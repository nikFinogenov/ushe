#include "ush.h"

void echo(char* line) {
    char* res = NULL;
    char* str_flags = parse_flags(line, &res);
    t_echo_flags_s flags;
    init_echo_flags(&flags, str_flags);
    mx_strcpy(line, res);

    printf("%s\n", &line[5]);
    // mx_printstr(&line[5]);
    // mx_printchar('\n');
}
