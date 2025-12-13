
volatile unsigned int ticks = 0;

unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void send_eoi() {
    outb(0x20, 0x20);
}

void init_pit() {
    unsigned int divisor = 1193182 / 100; 
    outb(0x43, 0x34);                    
    outb(0x40, divisor & 0xFF);           
    outb(0x40, divisor >> 8);             
}

void pit_handler() {
    ticks++;        
    send_eoi();     
}

void enable_interrupts() {
    asm volatile("sti");
}

void sleep(unsigned int ms) {
    if(ms == 0) return;
    unsigned int target = ticks + (ms / 10);
    while(ticks < target) {
        asm volatile("hlt"); 
    }
}
unsigned char get_scancode() {
    return inb(0x60);
}

char wait_for_key() {
    unsigned char scancode;
    while(1) {
        scancode = get_scancode();
        if(!(scancode & 0x80)) {
            return scancode;
        }
    }
}

void cpu_sleep(unsigned int ms) {
    unsigned long long cycles = ms * 1000;
    unsigned long long i;
    for(i = 0; i < cycles; i++) {
        asm volatile("pause");
    }
}
