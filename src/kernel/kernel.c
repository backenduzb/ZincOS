#include "functions/c/string/functions.h"
#include "functions/c/screen/functions.h"
#include "config/colors.h"
#include "keyboard/keyboard.h"
#include "timing/sleep.h"
#include "storage/ata.h"

extern void shutdown();

static int streq(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

static int starts_with(const char *s, const char *prefix)
{
    while (*prefix)
    {
        if (*s++ != *prefix++)
        {
            return 0;
        }
    }
    return 1;
}

void kernel_main(unsigned int magic, unsigned int addr)
{
    char hex_buf[20];
    const char *hex_chars = "0123456789ABCDEF";
    hex_buf[0] = '0';
    hex_buf[1] = 'x';

    for (int i = 0; i < 8; i++)
    {
        hex_buf[9 - i] = hex_chars[(magic >> (i * 4)) & 0xF];
    }
    hex_buf[10] = '\0';

    print_string(hex_buf, 13, 0, WHITE_ON_BLACK);
    if (magic == 0x2BADB002)
    {
        print_string(" - MULTIBOOT OK", 23, 0, GREEN_ON_BLACK);
        print_string("Zinc OS Initialized", 0, 2, CYAN_ON_BLACK);
    }
    else
    {
        print_string(" - BAD MAGIC", 23, 0, RED_ON_BLACK);
        print_string("Expected: 0x2BADB002", 0, 2, RED_ON_BLACK);
        print_string("Kernel may still work...", 0, 3, YELLOW_ON_BLACK);
    }

    char key_counter[256];
    unsigned char cfg_sector[512];
    int shown = 0;
    int current_line = 6;

    strcpy(key_counter, "[root@zinc] - $ ");
    int key_idx = 16;
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
                const char *cmd = key_counter + 16;
                int output_line = current_line + 1;
                if (streq(cmd, "clear"))
                {
                    current_line = 0;
                    clear_screen();
                }
                else if (streq(cmd, "mdown"))
                {
                    shutdown();
                }
                else if (streq(cmd, "loadcfg"))
                {
                    int rc = ata_read_sector(100, cfg_sector);
                    if (rc == 0 && cfg_sector[0] == 'Z' && cfg_sector[1] == 'C' &&
                        cfg_sector[2] == 'F' && cfg_sector[3] == 'G')
                    {
                        cfg_sector[511] = 0;
                        clear_line(output_line);
                        print_string((char *)(cfg_sector + 4), 1, output_line, WHITE_ON_BLACK);
                    }
                    else
                    {
                        clear_line(output_line);
                        print_string("CONFIG READ ERROR", 1, output_line, RED_ON_BLACK);
                    }
                }
                else if (starts_with(cmd, "savecfg "))
                {
                    for (int i = 0; i < 512; i++)
                    {
                        cfg_sector[i] = 0;
                    }
                    cfg_sector[0] = 'Z';
                    cfg_sector[1] = 'C';
                    cfg_sector[2] = 'F';
                    cfg_sector[3] = 'G';
                    int idx = 8;
                    int out = 4;
                    while (cmd[idx] && out < 511)
                    {
                        cfg_sector[out++] = (unsigned char)cmd[idx++];
                    }
                    int rc = ata_write_sector(100, cfg_sector);
                    clear_line(output_line);
                    if (rc == 0)
                    {
                        print_string("CONFIG SAVED", 1, output_line, GREEN_ON_BLACK);
                    }
                    else
                    {
                        print_string("CONFIG WRITE ERROR", 1, output_line, RED_ON_BLACK);
                    }
                }

                strcpy(key_counter, "[root@zinc] - $ ");
                key_idx = 16;
                current_line += 2;
            }
            else if (key == 8 && key_idx > 16)
            {
                key_idx--;
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
