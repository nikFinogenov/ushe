#include "ush.h"

static void export_var(void) {
    for (char **env = environ; *env != NULL; env++) {
        char *name = *env;
        char *value = strchr(name, '=');
        if (value != NULL) {
            *value = '\0';
            value++;
        }
        printf("declare -x %s=\"%s\"\n", name, value);
    }
}

int export(char* arguments) {

    char* variable = NULL;
    char* value = NULL;
    char *token = strtok(arguments + 7, " ");

    if (token == NULL) {
        export_var();
        return 1;
    }

    while (token != NULL) {
        char *equal_sign = strchr(token, '=');

        if (equal_sign != NULL) {
            *equal_sign = '\0';
            variable = token;
            value = equal_sign + 1;
            setenv(variable, value, 1);
        } else {
            setenv(token, "", 1);
        }

        token = strtok(NULL, " ");
    }

    return 0;
}
