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

start:
    cli

    push eax       
    push ebx       

    lgdt [gdt_descriptor]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_cs

flush_cs:
    mov esp, stack_top

    pop ebx         
    pop eax         

    push eax        
    push ebx        
    call kernel_main

    cli
    hlt

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
