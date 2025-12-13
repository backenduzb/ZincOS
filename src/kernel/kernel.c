#include "config/colors.c"
#include "config/settings.c"
#include "config/functions.c"
#include "keyboard/keyboard.c"
#include "timing/sleep.c"

void kernel_main(unsigned int magic, unsigned int addr)
{
    print_string("Loading kernel ...", 0, 0, WHITE_ON_BLACK);
    cpu_sleep(10000);
    if (magic == 0x2BADB002)
        print_string("Succes Loaded Zinc", 0, 0, WHITE_ON_BLACK);
    else
        print_string("Have an error !", 0, 0, RED_ON_BLACK);

    cpu_sleep(10000);
    char key_counter[256];
    int key_idx = 16;
    strcpy(key_counter, "[root@zinc] - $ ");
    int shown = 0;
    int current_line = 5;

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
                    current_line++;
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
