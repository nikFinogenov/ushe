#include "../inc/libmx.h"

int mx_str_contains(const char *str, char target_letter) {
    while (*str) {
        if (*str == target_letter) {
            return 1; 
        }
        str++;
    }
    return 0; 
}
