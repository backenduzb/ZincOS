#include "settings.c"
#include "colors.c"

void clear_screen() {
    char *vga = (char*)VGA_ADDRESS;
    for (int i = 0; i < WIDTH * HEIGHT * 2; i += 2) {
        vga[i] = ' ';
        vga[i + 1] = CYAN_ON_BLACK;
    }
}

void print_string(const char *str, int x, int y, char color) {
    unsigned short *vga_buffer = (unsigned short*)VGA_ADDRESS;
    int position = y * WIDTH + x;
    
    for (int i = 0; str[i] != '\0'; i++) {
        vga_buffer[position + i] = (unsigned short)str[i] | (color << 8);
    }
}