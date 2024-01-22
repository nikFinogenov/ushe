#include "ush.h"

int main(void) {
    init();
    while(1) {
        if(isatty(0)) mx_printstr("u$h> ");
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
            char* command = mx_strsplit(commands[i], ' ')[0];
            if (strcmp(command, "pwd") == 0) pwd();
            else if (strcmp(command, "export") == 0) export(commands[i]);
            else if (strcmp(command, "unset") == 0) unset(commands[i]);
            else if (strcmp(command, "cd") == 0) cd(mx_strsplit(commands[i], ' ')[1]);
            else if (strcmp(command, "echo") == 0) echo(commands[i]);
            else if (strcmp(command, "exit") == 0) my_exit();//somehow need to manage status
            else printf("u$h: command not found: %s\n", commands[i]);
        }
    }
}
