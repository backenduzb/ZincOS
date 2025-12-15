bits 32
global idt_descriptor

default_interrupt:
    pusha                  
    hlt
    popa
    iretd                  


section .idt
align 8
idt_table:
times 256 dq default_interrupt 

idt_descriptor:
    dw idt_table_end - idt_table - 1   
    dd idt_table                        

idt_table_end: