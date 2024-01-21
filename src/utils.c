#include "ush.h"

extern char* HOME;
extern char* PWD;
extern char* PREVPWD;

void init(void) {
    HOME = getpwuid(getuid())->pw_dir;
    PWD = getcwd(NULL, 1024);
    PREVPWD = getcwd(NULL, 1024);
}

char* replace_tilda(char* argument, int *flag) {
    int tildeIndex = mx_get_char_index(argument, '~');
    
    if (tildeIndex != -1) {
        if (argument[tildeIndex + 1] == '+'){
            *flag = 1;
            return mx_replace_sub_string(argument, "~+", PWD);
        }
            
        else if (argument[tildeIndex + 1] == '-') {
            *flag = 2;
            return mx_replace_sub_string(argument, "~-", PREVPWD);
        }
        else {
            *flag = 0;
            return mx_replace_sub_string(argument, "~", HOME);
        }
    }
    return argument;
}

char* replace_tilda_backwards(char* argument, int flag) {
    switch (flag) {
    case 0:
        return mx_replace_sub_string(argument, HOME, "~");
        break;
    case 1:
        return mx_replace_sub_string(argument, PWD, "~+");
        break;
    case 2:
        return mx_replace_sub_string(argument, PREVPWD, "~-");
        break;
    default:
        break;
    }
    return argument;
}

char* command_format(char* command) {
    char* token = strtok(command, " \t\n\r\f");
    
    char* temp = NULL;

    while (token != NULL) {
        temp = mx_strjoin(temp, token);
        token = strtok(NULL, " \t\n\r\f");
        if (token != NULL) temp = mx_strjoin(temp, " ");
        
    }
    return temp;
}
