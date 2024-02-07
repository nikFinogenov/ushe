#include "ush.h"

static int count_quotes(char* line) {
    int count = 0;
    for(int i = 0; i < mx_strlen(line); i++) {
        if (line[i] == '\'' || line[i] == '\"') {
            if (line[i - 1] != '\\') count++;
        }
    }
    return count;
}
static void remove_quotes(char* str) {
    if (str == NULL) {
        return;
    }

    int readIndex = 0;
    int writeIndex = 0;

    while (str[readIndex] != '\0') {
        if (str[readIndex] != '\'' || str[readIndex] != '\"') {
            if(str[readIndex - 1] )
        }

        readIndex++;
    }

    str[writeIndex] = '\0';
}

void echo(char* line) {
    char* res = NULL;
    char* str_flags = parse_flags(line, &res);
    t_echo_flags_s flags;
    init_echo_flags(&flags, str_flags);
    mx_strcpy(line, res);
    count_quotes(line);
    printf("%s\n",&line[5]);
    // printf("%s\n",line);

    if(count_quotes(&line[5]) % 2 == 0) {
        // mx_remove_symbol(line, '\"');
        // mx_remove_symbol(line, '\'');
        printf("%s", &line[5]);

        if(!flags.n) {
            printf("\n");
        }
    } else {
        mx_printerr("pidoras normalno skobochki postav\n");
    }
}
