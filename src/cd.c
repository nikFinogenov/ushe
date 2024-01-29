#include "ush.h"
#include <sys/stat.h>
#include <limits.h>
bool is_link(const char *filename) {
    struct stat st;
    printf("%s\n", filename);
    if (lstat(filename, &st) == 0) {
        return S_ISLNK(st.st_mode);
    }
    return false;
}
char* get_absolute_path(const char* pwd, const char* filename) {
    size_t len_pwd = strlen(pwd);
    size_t len_filename = strlen(filename);

    // Allocate memory for the combined path
    char* absolute_path;

    // Check if the filename is already an absolute path
    if (filename[0] == '/' || filename[0] == '~') {
        absolute_path = strdup(filename);
    } else {
        absolute_path = (char*)malloc(len_pwd + len_filename + 2);  // +2 for '/' and null terminator

        if (absolute_path == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        // Combine the current working directory and the filename
        strcpy(absolute_path, pwd);

        // Make sure there is a '/' between pwd and filename
        if (pwd[len_pwd - 1] != '/') {
            strcat(absolute_path, "/");
        }

        strcat(absolute_path, filename);
    }

    return absolute_path;
}
void remove_last_component(char* path) {
    if (path != NULL) {
        size_t len = strlen(path);

        if (len > 0) {
            // Find the last occurrence of '/'
            char* last_slash = strrchr(path, '/');

            if (last_slash != NULL) {
                // Check if it's the only character in the string
                if (last_slash == path && len == 1) {
                    // If it's the root directory, leave it unchanged
                    return;
                }

                // Truncate the string at the last '/'
                *last_slash = '\0';

                // Ensure the result is at least the root directory ("/")
                if (strlen(path) == 0) {
                    strcpy(path, "/");
                }
            }
        }
    }
}
void cd(char* command) {
    char* res = NULL;
    char* str_flags = parse_flags(command, &res);
    t_cd_flags_s flags;
    init_cd_flags(&flags, str_flags);
    mx_strcpy(command, res);

    char* path = mx_strsplit(command, ' ')[1];
    if(path[mx_strlen(path) - 1] == '/') path[mx_strlen(path) - 1] = '\0';
    if(mx_get_length(mx_strsplit(command, ' ')) <= 2){
        if (path == NULL) {
            path = HOME;
        }
        if(mx_strcmp(path, "-") == 0) {
            path = PREVPWD;
        }
        if(mx_strcmp(path, "..") == 0) {
        //     path = PREVPWD;
            remove_last_component(PWD);
        }
        struct stat path_stat;
        if (lstat(path, &path_stat) == 0) {
            // if (S_ISLNK(path_stat.st_mode)) {
            //     PREVPWD = PWD;
            //     setenv("OLDPWD", PREVPWD, 1);
            //     PWD = get_absolute_path(PWD, path);
            //     setenv("PWD", PWD, 1);
            // }
            // else {
                if (chdir(path) == 0) {
                    PREVPWD = PWD;
                    setenv("OLDPWD", PREVPWD, 1);
                    // printf("%s\n", path);
                    // PWD = is_link(path) ? get_absolute_path(PWD, path) : getcwd(NULL, 1024);
                    PWD = S_ISLNK(path_stat.st_mode) ? get_absolute_path(PWD, path) : getcwd(NULL, 1024);
                    setenv("PWD", PWD, 1);
                } else {
                    perror("chdir");
                    return;
                }
            }
        // }
    }
    else{
        mx_printerr("ush: cd: string not in pwd: ");
        mx_printerr(path);
        mx_printerr("\n");
    }
}
