#include "functions.h"

int strcmp(const char *a, const char *b) {
    a += 16;
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *a - *b;
}

void strcpy(char *dest, const char *src) {
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}