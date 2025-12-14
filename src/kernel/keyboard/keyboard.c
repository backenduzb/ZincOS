#include "keyboard.h"

extern char get_key();  

char scancode_to_char(char key) {
    switch(key) {
        //String keys
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
        //Space button
        case 0x39: return ' ';
        //Backspace button    
        case 0x0E: return 8;      
        case 0x0F: return 9;
        //Enter button
        case 0x1C: return '\n';
        //Ctrl buttons LR
        case 0x1D: return 10;
        case 0xE01D: return 11;
        //Shift buttons LR
        case 0x2A: return 12;
        case 0x36: return 13;
        //Alt buttons LR
        case 0x38: return 14;
        case 0xE038: return 15;
        //Super buttons LR
        case 0xE05B: return 16;
        case 0xE05C: return 17;
        //Menu button
        case 0xE05D: return 18;
        //Digit buttons
        case 0x0B: return '0';
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
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
