#include "config/colors.c"
#include "config/settings.c"
#include "config/functions.c"

void kernel_main(unsigned int magic, unsigned int addr) {
    clear_screen();
    
    if (magic == 0x2BADB002) {
    print_string("Succes Loaded Zinc", 0, 0, WHITE_ON_BLACK);
    } else {
        print_string("Have an error !", 0, 0, RED_ON_BLACK);
    }
    print_string("==========================", 0, 1, CYAN_ON_BLACK);
    print_string("|   Welcome, to ZINC !   |", 0, 2, CYAN_ON_BLACK);
    print_string("==========================", 0, 3, CYAN_ON_BLACK);
        
    while(1); 
}
