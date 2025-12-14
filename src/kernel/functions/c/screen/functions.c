#include "../../../config/settings.h"
#include "../../../config/colors.h"
#include "functions.h"

void clear_screen() {
    char *vga = (char*)VGA_ADDRESS;
    for (int i = 0; i < WIDTH * HEIGHT * 2; i += 2) {
        vga[i] = ' ';
        vga[i + 1] = BLACK_ON_BLACK;
    }
}

void clear_line(int line){
    if (line == 0){
        line = 1;
    }
    char *vga = (char*) VGA_ADDRESS;
    for (int i = (line * WIDTH)*2; i < ((WIDTH * line) + WIDTH)*2 ; i += 2){
        vga[i] = ' ';
        vga[i + 2] = BLACK_ON_BLACK;
    }
}

void print_string(const char *str, int x, int y, char color) {
    unsigned short *vga_buffer = (unsigned short*)VGA_ADDRESS;
    int position = y * WIDTH + x;

    for (int i = 0; str[i] != '\0'; i++) {
        vga_buffer[position + i] = (unsigned short)str[i] | (color << 8);
    }
}

void print_char_at(char c, int x, int y, int color) {
    unsigned short *video = (unsigned short*)VGA_ADDRESS;
    video[y * 80 + x] = (color << 8) | c;
}