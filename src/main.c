#include "ush.h"


int main(void) {
    init();
    while(1) {
        if(isatty(0)) printf("%s", "u$h> ");
        char* line = NULL;
        size_t length = 0;

        if (getline(&line, &length, stdin) == -1) {
            free(line); 
            exit(1);
        }
        line[strcspn(line, "\n")] = '\0';
        char** commands = mx_strsplit(line, ';');
        mx_strdel(&line);
        for (int i = 0; commands[i] != NULL; ++i) {
            mx_strcpy(commands[i], mx_strtrim(command_format(commands[i])));
            int flag = -1;
            char* repl = replace_tilda(commands[i], &flag);
            size_t secondStringLength = strlen(repl);
            char* destinationBuffer = (char*)malloc(secondStringLength + 1);
            mx_strcpy(destinationBuffer, repl);
            commands[i] = destinationBuffer;

            char* command = mx_strsplit(commands[i], ' ')[0];
            if (strcmp(command, "pwd") == 0) pwd(commands[i]);
            else if (check_buildin(command)) system(commands[i]);
            else if (strcmp(command, "export") == 0) export(commands[i]);
            else if (strcmp(command, "unset") == 0) unset(commands[i]);
            else if (strcmp(command, "which") == 0) which(commands[i]); 
            else if (strcmp(command, "env") == 0) env(commands[i]);          
            else if (strcmp(command, "cd") == 0) cd(commands[i]);
            else if (strcmp(command, "echo") == 0) echo(commands[i]);
            else if (strcmp(command, "exit") == 0) my_exit();
            else {
                if(flag != -1) command = replace_tilda_backwards(command, flag);
                printf("u$h: %s: command not found\n", command);
            }
        }
    }
}
