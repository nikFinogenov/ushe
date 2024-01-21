#include "ush.h"

extern char* HOME;

void init(void) {
    // struct passwd *pw = ;
    HOME = getpwuid(getuid())->pw_dir;
}

char* replace_tilda(char* argument) {
    return mx_replace_substr(argument, "~", HOME);
}

char* replace_tilda_backwards(char* argument) {
    return mx_replace_substr(argument, HOME, "~");
}

char* command_format(char* command) {
    char* token = strtok(command, " \t\n\r\f");
    
    char* temp = NULL;

    while (token != NULL) {
        // printf("%s\n", token);
        temp = mx_strjoin(temp, token);
        token = strtok(NULL, " \t\n\r\f");
        if (token != NULL) temp = mx_strjoin(temp, " ");
        
    }
    return temp;
}
