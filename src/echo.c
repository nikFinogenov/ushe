#include "ush.h"

void echo(char* line) {
    printf("%s\n", &line[5]);
    // mx_printstr(&line[5]);
    // mx_printchar('\n');
}
