#include "ush.h"

int which(char* arguments) {
    char* res = NULL;
    char* str_flags = parse_flags(arguments, &res);
    t_which_flags_s flags;
    init_which_flags(&flags, str_flags);
    mx_strcpy(arguments, res);

    const char* which_command = "which";
    if (strncmp(arguments, which_command, strlen(which_command)) == 0) {
        char* commands = arguments + strlen(which_command);
        char* command = strtok(commands, " ");

        while (command != NULL) {
            if (command[0] != '-') {
                char* path = strdup(PATH);
                char* found_cmd = find_in_path(command, &path);
                if (found_cmd != NULL) {
                    printf("%s\n", found_cmd);
                    free(found_cmd);
                } else if (!check_buildin(command)) {
                    printf("%s: buildin command\n", command);
                } else {
                    printf("%s: command not found\n", command);
                }

                free(path);      
                command = strtok(NULL, " ");
            }
        }
    }
    return 0;
}
