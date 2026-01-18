#include "keyboard.h"

char scancode_to_char(unsigned char scancode) {
    // Faqat bosilish (key press) holatini qabul qilamiz
    if (scancode & 0x80) {
        return 0;  // Key release - e'tiborsiz qoldiramiz
    }
    
    switch(scancode) {
        case 0x1E: return 'a'; case 0x30: return 'b'; case 0x2E: return 'c';
        case 0x20: return 'd'; case 0x12: return 'e'; case 0x21: return 'f';
        case 0x22: return 'g'; case 0x23: return 'h'; case 0x17: return 'i';
        case 0x24: return 'j'; case 0x25: return 'k'; case 0x26: return 'l';
        case 0x32: return 'm'; case 0x31: return 'n'; case 0x18: return 'o';
        case 0x19: return 'p'; case 0x10: return 'q'; case 0x13: return 'r';
        case 0x1F: return 's'; case 0x14: return 't'; case 0x16: return 'u';
        case 0x2F: return 'v'; case 0x11: return 'w'; case 0x2D: return 'x';
        case 0x15: return 'y'; case 0x2C: return 'z'; case 0x39: return ' ';
        case 0x0E: return 8;  case 0x0F: return 9;   case 0x1C: return '\n';
        default: return 0;
    }
}

char wait_for_key_single(void) {
    unsigned char sc = keyboard_scancode;
    if (sc != 0) {
        keyboard_scancode = 0;  // clear qilamiz
        return scancode_to_char(sc);
    }
    return 0;
}
