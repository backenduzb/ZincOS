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

void hex_string(unsigned int n, char* buffer) {
    const char* digits = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';
    
    for(int i = 0; i < 8; i++) {
        buffer[9 - i] = digits[(n >> (i * 4)) & 0xF];
    }
    buffer[10] = '\0';
}