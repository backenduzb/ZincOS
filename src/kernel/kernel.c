#include "functions/c/string/functions.h"
#include "functions/c/screen/functions.h"
#include "config/colors.h"
#include "keyboard/keyboard.h"
#include "timing/sleep.h"

extern void shutdown();

void kernel_main(unsigned int magic, unsigned int addr)
{
    // print_string("Loading kernel ...", 0, 0, WHITE_ON_BLACK);

    // if (magic == 0x2BADB002)
    //     print_string("Succes Loaded Zinc", 0, 0, WHITE_ON_BLACK);
    // else
    //     print_string("Have an error !", 0, 0, RED_ON_BLACK);

    char hex_buf[20];
    const char* hex_chars = "0123456789ABCDEF";
    hex_buf[0] = '0';
    hex_buf[1] = 'x';
    
    for (int i = 0; i < 8; i++) {
        hex_buf[9 - i] = hex_chars[(magic >> (i * 4)) & 0xF];
    }
    hex_buf[10] = '\0';
    
    print_string(hex_buf, 13, 0, WHITE_ON_BLACK);
    
    // MULTIBOOT 1 uchun TO'G'RI tekshirish
    if (magic == 0x2BADB002) {  // <<< TO'G'RI! Kernelga 0x2BADB002 keladi
        print_string(" - MULTIBOOT OK", 23, 0, GREEN_ON_BLACK);
        print_string("Zinc OS Initialized", 0, 2, CYAN_ON_BLACK);
    } else {
        print_string(" - BAD MAGIC", 23, 0, RED_ON_BLACK);
        print_string("Expected: 0x2BADB002", 0, 2, RED_ON_BLACK);
        print_string("Kernel may still work...", 0, 3, YELLOW_ON_BLACK);
    }
    char key_counter[256];
    int key_idx = 16;
    strcpy(key_counter, "[root@zinc] - $ ");
    int shown = 0;
    int current_line = 6;

    print_string(key_counter, 1, current_line, WHITE_ON_BLACK);
    while (1)
    {
        if (!shown)
        {
            print_string("==========================", 0, 2, CYAN_ON_BLACK);
            print_string("|   Welcome, to ZINC !   |", 0, 3, WHITE_ON_BLACK);
            print_string("==========================", 0, 4, CYAN_ON_BLACK);
            shown = 1;
        }
        char key = wait_for_key_single();

        if (key != 0)
        {
            if (key == '\n')
            {
                if (strcmp(key_counter, "clear") == 0)
                {
                    current_line = 0;
                    clear_screen();
                    int key_idx = 16;
                    strcpy(key_counter, "[root@zinc] - $ ");
                }
                if (strcmp(key_counter, "mdown") == 0)
                {
                    shutdown();
                }
                else
                {
                    strcpy(key_counter, "[root@zinc] - $ ");
                    key_idx = 16;
                    current_line +=2;
                }
            }
            else if (key == 8 && key_idx >= 16)
            {
                if (key_idx > 16)
                {
                    key_idx--;
                }
                key_counter[key_idx] = 0;
            }
            else
            {
                key_counter[key_idx++] = key;
                key_counter[key_idx] = 0;
            }
            clear_line(current_line);
            print_string(key_counter, 1, current_line, WHITE_ON_BLACK);
        }

        cpu_sleep(100);
    }
}
