#ifndef SLEEP_H
#define SLEEP_H

void inb();
void outb();
void send_eoi();
void init_pit();
void pit_handler();
void enabler_interrupts();
void sleep(unsigned int ms);
unsigned char get_scancode();
char wait_for_key();
void cpu_sleep(unsigned int ms);

#endif