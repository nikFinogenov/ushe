#include "ush.h"

void echo(char* line) {
    char* res = NULL;
    char* str_flags = parse_flags(line, &res);
    t_echo_flags_s flags;
    init_echo_flags(&flags, str_flags);
    mx_strcpy(line, res);
    
    // printf("%s", &line[5]);
    mx_printstr(&line[5]);
    if(!flags.n) mx_printchar('\n');
    // mx_printstr(&line[5]);
    // mx_printchar('\n');
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
