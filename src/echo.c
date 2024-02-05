#include "ush.h"

static int count_quotes(char* line) {
    int count = 0;
    for(int i = 0; i < mx_strlen(line); i++) {
        if (line[i] == '\'' || line[i] == '\"') count++;
    }
    return count;
}
// void replace_escape_sequences(char* str) {
//     char* input = str;
//     char* output = str;

//     while (*input) {
//         if (*input == '\\' && *(input + 1) != '\0') {
//             switch (*(input + 1)) {
//                 case 'a': *output++ = '\a'; break;
//                 case 'b': *output++ = '\b'; break;
//                 case 'c': /* suppress final newline, break or return as needed */ break;
//                 case 'e': *output++ = 27; break;
//                 case 'f': *output++ = '\f'; break;
//                 case 'n': *output++ = '\n'; break;
//                 case 'r': *output++ = '\r'; break;
//                 case 't': *output++ = '\t'; break;
//                 case 'v': *output++ = '\v'; break;
//                 case '\\': *output++ = '\\'; break;
//                 case '0':  // Octal representation
//                     sscanf(input + 1, "%s", &(*output));
//                     while (*output) output++;
//                     break;
//                 case 'x':  // Hexadecimal representation
//                     sscanf(input + 1, "%s", &(*output));
//                     while (*output) output++;
//                     break;
//                 case 'u':  // Unicode (4-digit)
//                     // Handle Unicode escape sequence
//                     break;
//                 case 'U':  // Unicode (8-digit)
//                     // Handle Unicode escape sequence
//                     break;
//                 default: *output++ = *(input + 1);
//             }
//             input += 2; // Skip the escape sequence
//         } else {
//             *output++ = *input++;
//         }
//     }

//     *output = '\0'; // Null-terminate the modified string
// }

void echo(char* line) {
    char* res = NULL;
    char* str_flags = parse_flags(line, &res);
    t_echo_flags_s flags;
    init_echo_flags(&flags, str_flags);
    mx_strcpy(line, res);
    count_quotes(line);
    printf("%s\n",&line[5]);

    // if(count_quotes(&line[5]) % 2 == 0) {
    //     mx_remove_symbol(line, '\"');
    //     mx_remove_symbol(line, '\'');
    //     // replace_escape_sequences(&line[5]);
    //     printf("%s", &line[5]);

    //     if(!flags.n) {
    //         printf("\n");
    //     }
    // } else {
    //     mx_printerr("pidoras normalno skobochki postav\n");
    // }
}

// static void edit_argv(int n_options) {
//     // echo_t echo_options = {0, 0, 0};

//     // fill_options(p->argv, &echo_options, n_options);
//     for(int i = n_options + 1; p->argv[i] != NULL; i++) {
//         if (!echo_options.E && strstr(p->argv[i], "\\")) {
//             mx_escape_seq(p, i, echo_options);
//         }
//         printf("%s",p->argv[i]);
//         if (strstr(p->argv[i],"\\c"))
//             break;
//         if (p->argv[i + 1])
//             mx_printstr(" ");
//     }
//     !echo_options.n ? printf("\n") : 0;
// }

// int mx_echo(t_shell *m_s, t_process *p) {
//     m_s->exit_code = 0;
//     int exit_code = m_s->exit_code;
//     int n_options = count_options(p->argv);

//     edit_argv(n_options, p);
//     return exit_code;
// }
