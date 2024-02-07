// #include "ush.h"

// static int count_quotes(char* line) {
//     int count = 0;
//     for(int i = 0; i < mx_strlen(line); i++) {
//         if (line[i] == '\'' || line[i] == '\"') {
//             if (line[i - 1] != '\\') count++;
//         }
//     }
//     return count;
// }
// static void remove_quotes(char* input) {
//     size_t input_length = strlen(input);
//     size_t output_index = 0;
//     int inside_quote = 0;

//     for (size_t i = 0; i <= input_length; i++) {
//         if (input[i] == '\'' || input[i] == '"') {
//             if (i > 0 && input[i - 1] == '\\') {
//                 input[output_index++] = input[i];
//             } else {
//                 inside_quote = !inside_quote;
//             }
//         } else if (!inside_quote) {
//             input[output_index++] = input[i];
//         }
//     }

//     input[output_index] = '\0';
// }

// void echo(char* line) {
//     char* res = NULL;
//     char* str_flags = parse_flags(line, &res);
//     t_echo_flags_s flags;
//     init_echo_flags(&flags, str_flags);
//     mx_strcpy(line, res);
//     // count_quotes(line);
//     printf("%s\n",&line[5]);
//     // printf("%s\n",line);
//     // printf()
//     if(count_quotes(&line[5]) % 2 == 0) {
//         remove_quotes(line);
//         line = replace_escape_seq(line);
//         printf("%s", &line[5]);

//         if(!flags.n) {
//             printf("\n");
//         }
//     } else {
//         mx_printerr("pidoras normalno skobochki postav\n");
//     }
// }
#include "../inc/ush.h"
char *mx_delete_extra_spaces(const char *str) {
    if (!str)
        return NULL;
    char *s = mx_strnew(mx_strlen(str));
	char *rez;
    if (s)
        for (int k = 0; *str; s[k++] = *(str++))
            if (mx_isspace(*str)){
                for (s[k++] = ' '; mx_isspace(*str); ++str);
                if (!(*str))
                    break;
            }
    rez = mx_strtrim(s);
    free(s);
    return rez;
}

static void apply_escapes(char **str) {
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

static void del_extra_spaces(char **str) {
    if (strchr(*str, '"') || strchr(*str, '\''))
        return;
    *str = mx_delete_extra_spaces(*str);
}


int echo(char *line) {
    char* res = NULL;
    char* str_flags = parse_flags(line, &res);
    t_echo_flags_s flags;
    init_echo_flags(&flags, str_flags);
    mx_strcpy(line, res);
    del_extra_spaces(&line);

    *line = *mx_strtrim(line);
    apply_escapes(&line);

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
            char *path = mx_strdup(getenv("PWD"));
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
            mx_printerr("Odd number of quotes.\n");
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
            char *path = mx_strdup(getenv("PWD"));
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else {
            mx_strcat(str, ptr);
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
                mx_printchar('\n');
            }
        }
        else {
            mx_printerr("Odd number of quotes.\n");
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
            char *path = mx_strdup(getenv("PWD"));
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else {
            mx_strcat(str, ptr);
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
            if (isWrite) {
                mx_printstr(str);
                mx_printchar('\n');
            }
        }
        else {
            mx_printerr("Odd number of quotes.\n");
            mx_del_strarr(&data);
            return 1;
        }
    }
    /*
    else 
        mx_printchar('\n');
    */

    mx_del_strarr(&data);
    return 0;
}

