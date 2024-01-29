#include "ush.h"

extern char* HOME;
extern char* PWD;
extern char* PREVPWD;

char *buildin_list[19] = {"env", "export", "unset", "echo", "jobs", "fg", "bg",
                        "cd", "pwd", "which", "exit", "set", "kill", "chdir",
                        "true", "alias", "declare", "false", NULL};

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

char* parse_flags(char* command, char** result) {
    char **temp = mx_strsplit(command, ' ');
    int arg_length = mx_get_length(temp) - 1;
    char *flags = NULL;
    *result = mx_strdup(temp[0]);
    for(int i = 1; i <= arg_length; i++) {
        if((mx_strcmp(temp[0], "cd") == 0 && mx_strcmp(temp[i], "-") == 0) || temp[i][0] != '-') {
            *result = mx_strjoin(*result, " ");
            *result = mx_strjoin(*result, temp[i]);
        }
        else flags = mx_strjoin(flags, temp[i]+1);
    }
    return flags;
}

void init_cd_flags(t_cd_flags_s *flags, char* str_flags){
    flags->P = mx_str_contains(str_flags, 'P') ? true : false;
    flags->s = mx_str_contains(str_flags, 's') ? true : false;
}
void init_env_flags(t_env_flags_s *flags, char* str_flags){
    flags->i = mx_str_contains(str_flags, 'i') ? true : false;
    flags->P = mx_str_contains(str_flags, 'P') ? true : false;
    flags->u = mx_str_contains(str_flags, 'u') ? true : false;
}
int init_pwd_flags(t_pwd_flags_s *flags, char* str_flags) {
    if(str_flags != NULL) {
            for(int i = 0; i < mx_strlen(str_flags); i++){
                switch (str_flags[i])
                {
                case 'P':
                    flags->P = true;
                    flags->L = false;
                    break;
                case 'L':
                    flags->L = true;
                    flags->P = false;
                    break;
                default:
                    mx_printerr("ush: pwd: -");
                    mx_printerr(&str_flags[i]);
                    mx_printerr(": invalid option\npwd: usage: pwd [-LP]\n");
                    return 1;
                }
            }
    }
    return 0;
}
void init_which_flags(t_which_flags_s *flags, char* str_flags){
    flags->a = mx_str_contains(str_flags, 'a') ? true : false;
    flags->s = mx_str_contains(str_flags, 's') ? true : false;
}
void init_echo_flags(t_echo_flags_s *flags, char* str_flags){
    flags->E = mx_str_contains(str_flags, 'E') ? true : false;
    flags->e = mx_str_contains(str_flags, 'e') ? true : false;
    flags->n = mx_str_contains(str_flags, 'n') ? true : false;
}

bool check_buildin(char* command) {
    for (int i = 0; buildin_list[i] != NULL; ++i) {
        if (strcmp(buildin_list[i], command) == 0) {
            return false;
        }
    }
    return true;
}
