#include "ush.h"


static void spec_symbols(char **str) {
    char *ptr = mx_strchr(*str, '"');
    if (ptr != NULL) {
        char *slash_ptr = mx_strchr(ptr, '\\');
        while ( slash_ptr != NULL ) {
            switch (*(slash_ptr + 1))
            {
            case 'n':
                *slash_ptr = '\n';
                break;
            case 't':
                *slash_ptr = '\t';
                break;
            case '\\':
                *slash_ptr = '\\';
                break;
            case 'a':
                *slash_ptr = '\a';
                break;
            case 'v':
                *slash_ptr = '\v';
                break;
            case 'b':
                *slash_ptr = '\b';
                break;
            case 'f':
                *slash_ptr = '\f';
                break;
            case 'r':
                *slash_ptr = '\r';
                break;
            default:
                break;
            }
            
            slash_ptr++;
            *slash_ptr = '\0';
            for (; *(slash_ptr + 1) != '\0';) {
                mx_swap_char(slash_ptr, slash_ptr + 1);
                slash_ptr++;
            }
            slash_ptr = ptr + 1;

            if (mx_get_char_index(slash_ptr, '\\') > mx_get_char_index(slash_ptr, '"') || mx_get_char_index(slash_ptr, '\\') == -1) {
                ptr = mx_strchr(slash_ptr, '\'');
                break;
            }
            slash_ptr = mx_strchr(ptr, '\\');
        }
    }
    else
        ptr = mx_strchr(*str, '\'');

    if (ptr != NULL) {
        char *slash_ptr = mx_strchr(ptr, '\\');
        while ( slash_ptr != NULL ) {
            switch (*(slash_ptr + 1))
            {
            case 'n':
                *slash_ptr = '\n';
                break;
            case 't':
                *slash_ptr = '\t';
                break;
            case '\\':
                *slash_ptr = '\\';
                break;
            case 'a':
                *slash_ptr = '\a';
                break;
            case 'v':
                *slash_ptr = '\v';
                break;
            case 'b':
                *slash_ptr = '\b';
                break;
            case 'f':
                *slash_ptr = '\f';
                break;
            case 'r':
                *slash_ptr = '\r';
                break;
            default:
                break;
            }
            
            slash_ptr++;
            *slash_ptr = '\0';
            for (; *(slash_ptr + 1) != '\0';) {
                mx_swap_char(slash_ptr, slash_ptr + 1);
                slash_ptr++;
            }

            if (mx_get_char_index(slash_ptr, '\\') > mx_get_char_index(slash_ptr, '\'') || mx_get_char_index(slash_ptr, '\\') == -1)
                break;
            slash_ptr = mx_strchr(ptr, '\\');
        }
    }
}

static void extra_spaces(char **str) {
    if (strchr(*str, '"') || strchr(*str, '\'')) return;
    *str = mx_del_extra_spaces(*str);
}


int echo(char *line) {
    char* res = NULL;
    char* str_flags = parse_flags(line, &res);
    t_echo_flags_s flags;
    init_echo_flags(&flags, str_flags);
    mx_strcpy(line, res);
    extra_spaces(&line);

    *line = *mx_strtrim(line);
    spec_symbols(&line);

    char **data = mx_strsplit(line, '>');
    char *ptr = data[0];
    char *tmp_ptr = ptr;
    while (*tmp_ptr != '\0' && *tmp_ptr != ' ') {
        *tmp_ptr = '\0';
        for (; *(tmp_ptr + 1) != '\0';) {
            mx_swap_char(tmp_ptr, tmp_ptr + 1);
            tmp_ptr++;
        }
        tmp_ptr = ptr;
    }
    *tmp_ptr = '\0';
    for (; *(tmp_ptr + 1) != '\0';) {
        mx_swap_char(tmp_ptr, tmp_ptr + 1);
        tmp_ptr++;
    }
    tmp_ptr = ptr;
    if (*tmp_ptr == '-') {
        while (*tmp_ptr != '\0' && *tmp_ptr != ' ') {
            *tmp_ptr = '\0';
            for (; *(tmp_ptr + 1) != '\0';) {
                mx_swap_char(tmp_ptr, tmp_ptr + 1);
                tmp_ptr++;
            }
            tmp_ptr = ptr;
        }
    }
    if (*tmp_ptr == ' ') {
        *tmp_ptr = '\0';
        for (; *(tmp_ptr + 1) != '\0';) {
            mx_swap_char(tmp_ptr, tmp_ptr + 1);
            tmp_ptr++;
        }
        tmp_ptr = ptr;
    }

    tmp_ptr = ptr;

    if(flags.n) {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(4096);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(PWD);
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else {
            mx_strcat(str, data[0]);
        }
        int count = 0;
        bool parants = false;
        for (int i = 0; str[i]; i++) {
            if (str[i] == '"' || str[i] == '\'') {
                for (int j = i; str[j]; j++) {
                    str[j] = str[j + 1];
                }
                count++;
                parants = true;
            }
            else if (str[i] == '\\') {
                if (!parants) {
                    for (int j = i; str[j]; j++) {
                        str[j] = str[j+1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) {
            if(isWrite) {
                mx_printstr(str);
            }
        }
        else {
            mx_printerr("Norm kol-vo quotes postav\n");
            mx_del_strarr(&data);
            return 1;
        }
    }
    else if (flags.e || flags.E) {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(4096);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(PWD);
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else mx_strcat(str, ptr);

        int count = 0;
        bool parants = false;
        for (int i = 0; str[i]; i++) {
            if (str[i] == '"' || str[i] == '\'') {
                for (int j = i; str[j]; j++) {
                    str[j] = str[j + 1];
                }
                count++;
                parants = true;
            }
            else if (str[i] == '\\') {
                if (!parants) {
                    for (int j = i; str[j]; j++) str[j] = str[j+1];
                }
            }
        }
        if(count % 2 == 0 || count == 0) {
            if(isWrite) {
                mx_printstr(str);
                mx_printchar('\n');
            }
        }
        else {
            mx_printerr("Norm kol-vo quotes postav\n");
            mx_del_strarr(&data);
            return 1;
        }
    }
    else {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(4096);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(PWD);
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else mx_strcat(str, ptr);
        int count = 0;
        bool parants = false;
        for (int i = 0; str[i]; i++) {
            if (str[i] == '"' || str[i] == '\'') {
                for (int j = i; str[j]; j++) {
                    str[j] = str[j + 1];
                }
                count++;
                parants = true;
            }
            else if (str[i] == '\\') {
                if (!parants) {
                    for (int j = i; str[j]; j++) {
                        str[j] = str[j+1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) { 
            if (isWrite) {
                mx_printstr(str);
                mx_printchar('\n');
            }
        }
        else {
            mx_printerr("Norm kol-vo quotes postav\n");
            mx_del_strarr(&data);
            return 1;
        }
    }

    mx_del_strarr(&data);
    return 0;
}
