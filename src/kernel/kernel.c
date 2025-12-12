#include "config/colors.c"
#include "config/settings.c"
#include "config/functions.c"
#include "keyboard/keyboard.c"

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
            if (key == '\n' && strcmp(key_counter, "mdown") == 0){
                shutdown();
            }
            else if(key=='\n'){
                continue;
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
