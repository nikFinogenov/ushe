#include "ush.h"

static void export_var(void) {
    for (char **env = environ; *env != NULL; env++) {
        // char *equal_sign_pos = strchr(*env, '=');

        // if (equal_sign_pos != NULL) {
        //     size_t variableLength = equal_sign_pos - *env;

        //     char variable[variableLength + 1];
        //     strncpy(variable, *env, variableLength);
        //     variable[variableLength] = '\0';

        //     char *value = equal_sign_pos + 1;

        //     if (*value != '\0') {
        //         printf("declare -x %s=\"%s\"\n", variable, value);
        //     } else {
        //         printf("declare -x %s\n", variable);
        //     }
        // } ebal ya rot etoi ucode kontori
        printf("%s\n", *env);
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
