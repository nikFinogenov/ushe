#include "ush.h"

static char **split_string(char *str, char delimiter) {
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    while ((token = strchr(str, delimiter)) != NULL) {
        *token = '\0';
        tokens[position] = str;
        position++;

        if (position >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
        }

        str = token + 1;
    }

    tokens[position] = str;
    position++;

    tokens[position] = NULL;
    return tokens;
}

static char *find_in_path(char *cmd, char **path) {
    char **dirs = split_string(*path, ':');
    char *path_cmd = NULL;

    for (int i = 0; dirs[i] != NULL; i++) {
        path_cmd = malloc((strlen(dirs[i]) + strlen(cmd) + 2) * sizeof(char));
        sprintf(path_cmd, "%s/%s", dirs[i], cmd);

        if (access(path_cmd, X_OK) == 0) {
            free(dirs);
            return path_cmd;
        }

        free(path_cmd);
    }

    free(dirs);
    return NULL;
}

int which(char* arguments) {
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
                } else {
                    printf("%s: command not found\n", command);
                }
                free(path);
            }
            command = strtok(NULL, " ");
        }
    }
    return 0;
}
