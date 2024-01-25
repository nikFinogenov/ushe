#include "ush.h"

extern char* HOME;
extern char* PWD;
extern char* PREVPWD;

void init(void) {
    HOME = getpwuid(getuid())->pw_dir;
    PWD = getcwd(NULL, 1024);
    PREVPWD = getcwd(NULL, 1024);
    PATH = getenv("PATH");
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
        else if(argument[tildeIndex + 1] == '/' || argument[tildeIndex + 1] == '\0'|| argument[tildeIndex + 1] == ' '){
            *flag = 0;
            return mx_replace_sub_string(argument, "~", HOME);
        }
        else{

        const char* usernameEnd = strchr(argument, '/');

        size_t usernameLength = (usernameEnd != NULL) ? (size_t)(usernameEnd - argument - tildeIndex - 1) : strlen(argument);

        char* username = (char*)malloc(usernameLength + 1);

        strncpy(username, argument + tildeIndex + 1, usernameLength);
        username[usernameLength] = '\0';

            struct passwd *user_info = getpwnam(username);
            if (user_info != NULL) {
                char* path = mx_strjoin("~", username);
                return mx_replace_sub_string(argument, path, user_info->pw_dir);
            }
            
            return argument;
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

char* parse_flags(char* command) {
    char **temp = mx_strsplit(command, ' ');
    int arg_length = mx_get_length(temp) - 1;
    char *flags = NULL;
    for(int i = 1; i <= arg_length; i++) {
        if(temp[i][0] == '-') flags = mx_strjoin(flags, temp[i]+1);
    }
    return flags;
}

void init_cd_flags(t_cd_flags_s *flags){
    flags->hyphen = false; //хз флаг ли это, надо проверить
    flags->P = false;
    flags->s = false;
}
void init_env_flags(t_env_flags_s *flags){
    flags->i = false;
    flags->P = false;
    flags->u = false;
}
void init_pwd_flags(t_pwd_flags_s *flags, char* str_flags){
    flags->L = mx_str_contains(str_flags, 'L') ? true : false;
    flags->P = mx_str_contains(str_flags, 'P') ? true : false;
}
void init_which_flags(t_which_flags_s *flags){
    flags->a = false;
    flags->s = false;
}
void init_echo_flags(t_echo_flags_s *flags){
    flags->E = false;
    flags->e = false;
    flags->n = false;
}
