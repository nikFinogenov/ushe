#include "ush.h"

static void delete_last_part(char *path) {
    if (strcmp(path, "/") != 0) {
        char *lastSlash = strrchr(path, '/');

        if (lastSlash != NULL) {
            *lastSlash = '\0';
        }
    }
}

static bool check_for_link(char* path) {
    struct stat path_stat;
    char **tmp = mx_strsplit(path, '/');
    char *temp = NULL;
    for (int i = 0; i < mx_get_length(tmp); i++) {
        temp = mx_strjoin(temp, mx_strjoin("/", tmp[i]));
        if (lstat(temp, &path_stat) == 0) {
            if (S_ISLNK(path_stat.st_mode)) {
                mx_strdel(&temp);
                return true;
            }
        } else {
            mx_strdel(&temp);
            return false;
        }
    }
    mx_strdel(&temp);
    return false;
}

static bool is_dir(const char *path) {
    struct stat st;
    if (lstat(path, &st) == 0) {
        return S_ISDIR(st.st_mode) || S_ISLNK(st.st_mode);
    }
    return false;
}
static bool is_path_exists(const char *path) {
    if (access(path, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}
// static bool is_link(const char *path) {
//     struct stat st;
//     if (lstat(path, &st) == 0) {
//         return S_ISLNK(st.st_mode);
//     }
//     return false;
// }
void cd(char* command) {
    char* res = NULL;
    char* str_flags = parse_flags(command, &res);
    t_cd_flags_s flags;
    int status = init_cd_flags(&flags, str_flags);
    mx_strcpy(command, res);
    char *tmppwd = strdup(PWD);
    char* path = mx_strsplit(command, ' ')[1];
    if(status == 0) {
        if(path != NULL && path[mx_strlen(path) - 1] == '/') path[mx_strlen(path) - 1] = '\0';
        if(mx_get_length(mx_strsplit(command, ' ')) <= 2){
            if (path == NULL) {
                path = HOME;
            }
            if(mx_strcmp(path, "-") == 0) {
                path = PREVPWD;
            }
            if(path[0] != '/') {
                char** tmp = mx_strsplit(path, '/');    
                for(int i = 0; i < mx_get_length(tmp); i++) {
                    if(mx_strcmp(tmp[i], "..") == 0) {
                        delete_last_part(PWD);
                    }
                    else if(mx_strcmp(tmp[i], ".") == 0) continue;
                    else PWD = mx_strjoin(PWD, mx_strjoin("/", tmp[i]));
                }
            }
            if((flags.s && check_for_link(PWD)) || (!is_dir(PWD) && is_path_exists(PWD))) {
                PWD = tmppwd;
                mx_printerr("ush: cd: ");
                mx_printerr(path);
                mx_printerr(": Not a directory\n");
            }
            else if(chdir(PWD) == 0) {
                if(flags.P) {
                    printf("blya\n");
                    PWD = getcwd(NULL, 1024);
                }
                PREVPWD = tmppwd;
                if(path[0] == '/') PWD = path;
                setenv("OLDPWD", PREVPWD, 1);
                setenv("PWD", PWD, 1);
            }
            else {
                PWD = tmppwd;
                mx_printerr("ush: cd: ");
                mx_printerr(path);
                mx_printerr(": No such file or directory\n");
            }
        }
        else{
            mx_printerr("ush: cd: string not in pwd: ");
            mx_printerr(path);
            mx_printerr("\n");
        }
    }
}
