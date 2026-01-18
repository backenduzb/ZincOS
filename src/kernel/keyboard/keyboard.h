#ifndef KEYBOARD_H
#define KEYBOARD_H

extern volatile unsigned char keyboard_scancode;  // o'zgartirildi

char get_key(void);
char wait_for_key_single(void);
char scancode_to_char(unsigned char key);  // unsigned qiling

#endif