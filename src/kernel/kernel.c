#include "config/colors.c"
#include "config/settings.c"
#include "config/functions.c"

extern char get_key();  

char scancode_to_char(char key) {
    switch(key) {
        case 0x1E: return 'a';
        case 0x30: return 'b';
        case 0x2E: return 'c';
        case 0x20: return 'd';
        case 0x12: return 'e';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x17: return 'i';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x32: return 'm';
        case 0x31: return 'n';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x10: return 'q';
        case 0x13: return 'r';
        case 0x1F: return 's';
        case 0x14: return 't';
        case 0x16: return 'u';
        case 0x2F: return 'v';
        case 0x11: return 'w';
        case 0x2D: return 'x';
        case 0x15: return 'y';
        case 0x2C: return 'z';
        case 0x39: return ' ';    
        case 0x0E: return 8;      
        case 0x0F: return 9;
        case 0x1C: return '\n';

        default: return 0;        
    }
}



char wait_for_key_single() {
    static char last_key = 0;
    char sc = get_key(); 

    if (sc != 0 && sc != last_key) {
        last_key = sc;

        char key = scancode_to_char(sc);
        return key;  
    }

    if (sc == 0) {
        last_key = 0;
    }

    return 0;
}



void kernel_main(unsigned int magic, unsigned int addr)
{
    clear_screen();

    if (magic == 0x2BADB002)
        print_string("Succes Loaded Zinc", 0, 0, WHITE_ON_BLACK);
    else
        print_string("Have an error !", 0, 0, RED_ON_BLACK);

    clear_screen();
    
    char key_counter[256];
    int key_idx = 0;
    int shown = 0;

    while (1)
    {
        if (!shown)
        {
            print_string("==========================", 0, 1, CYAN_ON_BLACK);
            print_string("|   Welcome, to ZINC !   |", 0, 2, CYAN_ON_BLACK);
            print_string("==========================", 0, 3, CYAN_ON_BLACK);
            shown = 1;
        }

        char key = wait_for_key_single();

        if (key != 0) {

            if (key == '\n' && strcmp(key_counter, "clear") == 0 ){
                clear_screen();
                key_counter[0] = 0;
                key_idx = 0;
            }

            else if (key == 8 && key_idx > 0) {
                key_idx--;                
                key_counter[key_idx] = 0; 
                   
            }
            else{
                key_counter[key_idx++] = key;
                key_counter[key_idx] = 0; 
            }
            clear_screen();
            print_string(key_counter, 1, 1, WHITE_ON_BLACK);
        }
    }
}
