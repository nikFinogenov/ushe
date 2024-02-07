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
    int shlvl = mx_atoi(getenv("SHLVL"));
    setenv("SHLVL", mx_itoa(shlvl + 1), 1);
    char *shell = mx_strjoin(PWD, "/");
    shell = mx_strjoin(shell, "ush");
    setenv("SHELL", shell, 1);
    // TMP_X_Y = NULL;
}

char* replace_tilda(char* argument, int *flag) {
    int tildeIndex = mx_get_char_index(argument, '~');
    
    if (tildeIndex != -1 && argument[tildeIndex - 1] != '\\') {
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

int init_cd_flags(t_cd_flags_s *flags, char* str_flags){
    flags->L=false;
    flags->P=false;
    flags->s=false;
    if(str_flags != NULL) {
        for(int i = 0; i < mx_strlen(str_flags); i++){
            switch (str_flags[i])
            {
            case 'P':
                flags->P = true;
                break;
            case 'L':
                flags->L = true;
                break;
            case 's':
                flags->s = true;
                flags->P = false;
                break;
            default:
                mx_printerr("ush: cd: -");
                mx_printerr(&str_flags[i]);
                mx_printerr(": invalid option\ncd: usage: cd [-s] [-L|-P] [dir]\n");
                return 1;
            }
        }
    }
    return 0;
}
void init_env_flags(t_env_flags_s *flags, char* str_flags){
    flags->i = mx_str_contains(str_flags, 'i') ? true : false;
    flags->P = mx_str_contains(str_flags, 'P') ? true : false;
    flags->u = mx_str_contains(str_flags, 'u') ? true : false;
}
int init_pwd_flags(t_pwd_flags_s *flags, char* str_flags) {
    flags->L=false;
    flags->P=false;
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
    // flags->E=false;
    // flags->e=false;
    // flags->n=false;
    // if(str_flags != NULL) {
    //     for(int i = 0; i < mx_strlen(str_flags); i++){
    //         switch (str_flags[i])
    //         {
    //         case 'E':
    //             flags->E = true;
    //             break;
    //         case 'e':
    //             flags->e = true;
    //             break;
    //         case 'n':
    //             flags->n = true;
    //             break;
    //         default:
    //             mx_printerr("ush: echo: -");
    //             mx_printerr(&str_flags[i]);
    //             mx_printerr(": invalid option\ncd: usage: cd [-s] [-L|-P] [dir]\n");
    //             return 1;
    //         }
    //     }
    // }
    // return 0;
}

bool check_buildin(char* command) {
    for (int i = 0; buildin_list[i] != NULL; ++i) {
        if (strcmp(buildin_list[i], command) == 0) {
            return false;
        }
    }
    return true;
}

char *find_in_path(char *cmd, char **path) {
    char **dirs = mx_strsplit(*path, ':');
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

char* replace_escape_seq(const char* input) {
    size_t input_length = strlen(input);
    char* output = (char*)malloc(input_length + 1);

    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    size_t output_index = 0;
    int escaped = 0;

    for (size_t i = 0; i < input_length; i++) {
        if (escaped) {
            output[output_index++] = input[i];
            escaped = 0;
        } else if (input[i] == '\\') escaped = 1;
        else output[output_index++] = input[i];
    }

    output[output_index] = '\0';

    return output;
}
