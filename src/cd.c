#include "ush.h"
#include <sys/stat.h>
#include <limits.h>

// static char* get_absolute_path(const char* filename) {
//     size_t len_pwd = strlen(PWD);
//     size_t len_filename = strlen(filename);

//     char* absolute_path;

//     if (filename[0] == '/' || filename[0] == '~') {
//         absolute_path = strdup(filename);
//     } else {
//         absolute_path = (char*)malloc(len_pwd + len_filename + 2); 

//         if (absolute_path == NULL) {
//             perror("Memory allocation error");
//             exit(EXIT_FAILURE);
//         }

//         strcpy(absolute_path, PWD);

//         if (PWD[len_pwd - 1] != '/') {
//             strcat(absolute_path, "/");
//         }

//         strcat(absolute_path, filename);
//     }

//     return absolute_path;
// }

void delete_last_part(char *path) {
    if (strcmp(path, "/") != 0) {
        char *lastSlash = strrchr(path, '/');

        if (lastSlash != NULL) {
            *lastSlash = '\0'; // Set the last slash to null terminator to truncate the string.
        }
    }
}

// static bool check_for_link(char* path) {
//     struct stat path_stat;
//     char **tmp = mx_strsplit(path, '/');
//     char *temp = NULL;

//     for (int i = 0; i < mx_get_length(tmp); i++) {
//         temp = mx_strjoin(temp, mx_strjoin("/", tmp[i]));

//         if (lstat(temp, &path_stat) == 0) {
//             if (S_ISLNK(path_stat.st_mode)) {
//                 mx_strdel(&temp); // Cleanup allocated memory
//                 return true;
//             }
//         } else {
//             mx_strdel(&temp); // Cleanup allocated memory
//             return false;
//         }
//     }

//     mx_strdel(&temp); // Cleanup allocated memory
//     return false;
// }
// static char* manual_cwd(char* path) {
//     if(mx_strcmp(path, "..") != 0)
//         return mx_strjoin(PWD, mx_strjoin("/", path));
//     return PWD;
// }
// static char* join_pwd(char* path) {
//     if(path[0] == '/'){
//         return path;
//     }
//     return mx_strjoin(PWD, mx_strjoin("/", path));
// }

void cd(char* command) {
    char* res = NULL;
    char* str_flags = parse_flags(command, &res);
    t_cd_flags_s flags;
    init_cd_flags(&flags, str_flags);
    mx_strcpy(command, res);
    char *tmppwd = strdup(PWD);
    char* path = mx_strsplit(command, ' ')[1];

    if(path[mx_strlen(path) - 1] == '/') path[mx_strlen(path) - 1] = '\0';
    if(mx_get_length(mx_strsplit(command, ' ')) <= 2){
        // bool is_link = check_for_link(PWD);
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
                    // printf("PWD %s\n", PWD);
                }
                else if(mx_strcmp(tmp[i], ".") == 0) continue;
                else PWD = mx_strjoin(PWD, mx_strjoin("/", tmp[i]));
            }
        }
        // if(mx_strcmp(path, "..") == 0) {
        //     remove_last_component(PWD);
        //     is_link = check_for_link(PWD);
        //     if(!is_link) path = PWD;
        // }
        // struct stat path_stat;
        // if (lstat(path, &path_stat) == 0) {
        //     if (chdir(path) == 0) {
        //         PREVPWD = PWD;
        //         setenv("OLDPWD", PREVPWD, 1);
        //         if(S_ISLNK(path_stat.st_mode)) {
        //             PWD = join_pwd(path);
        //         }
        //         else if (is_link) {
        //             PWD = manual_cwd(path);
        //         }
        //         else PWD = getcwd(NULL, 1024);
        //         setenv("PWD", PWD, 1);
        //     } else {
        //         perror("chdir");
        //         return;
        //     }
        // }
        // printf("%s\n", PWD);
        if(chdir(PWD) == 0) {
            PREVPWD = tmppwd;
            if(path[0] == '/') PWD = path;
            // printf("PREV -> %s\n", PREVPWD);
            setenv("OLDPWD", PREVPWD, 1);
            setenv("PWD", PWD, 1);
        }
        else {
            PWD = tmppwd;
            mx_printerr("ush: cd: ");
            mx_printerr(path);
            mx_printerr(": No such file or directory\n");
        }
        // free(tmppwd);
    }
    else{
        mx_printerr("ush: cd: string not in pwd: ");
        mx_printerr(path);
        mx_printerr("\n");
    }

}
