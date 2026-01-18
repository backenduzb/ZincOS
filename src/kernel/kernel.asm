bits 32

section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002              
    dd 0x00000003              
    dd -(0x1BADB002 + 0x00000003) 

section .text
global start
extern kernel_main
extern gdt_descriptor
extern idt_descriptor
extern idt_init

start:
    mov esp, stack_top

    push ebx
    push eax

    cli

    lgdt [gdt_descriptor]

    mov ax, 0x10        
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov al, 0x11
    out 0x20, al       
    out 0xA0, al       

    mov al, 0x20
    out 0x21, al       
    mov al, 0x28
    out 0xA1, al       

    mov al, 0x04
    out 0x21, al       
    mov al, 0x02
    out 0xA1, al       

    mov al, 0x01
    out 0x21, al       
    mov al, 0x01
    out 0xA1, al       

    mov al, 0xFD        
    out 0x21, al
    mov al, 0xFF        
    out 0xA1, al

    jmp 0x08:flush_cs

flush_cs:
    call idt_init
    lidt [idt_descriptor]   
    sti

    call kernel_main

    cli
    hlt

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
