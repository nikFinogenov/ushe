#include "ush.h"

int unset(char* arguments) {
    char *token = strtok(arguments + 5, " ");

    while (token != NULL) {
        unsetenv(token);
        token = strtok(NULL, " ");
    }
    return 1;
}
