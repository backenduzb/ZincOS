bits 32
global idt_descriptor
global idt_init
extern default_interrupt
extern keyboard_interrupt

section .bss
align 8
idt_table: resb 256 * 8

section .data
idt_descriptor:
    dw (256 * 8) - 1
    dd idt_table

section .text
idt_init:
    pushad
    mov edi, idt_table
    mov ecx, 256
    mov eax, default_interrupt
.fill:
    call set_idt_entry
    add edi, 8
    loop .fill

    mov edi, idt_table
    add edi, 33 * 8
    mov eax, keyboard_interrupt
    call set_idt_entry
    popad
    ret

set_idt_entry:
    mov edx, eax
    mov word [edi], dx
    mov word [edi + 2], 0x08
    mov byte [edi + 4], 0
    mov byte [edi + 5], 0x8E
    shr edx, 16
    mov word [edi + 6], dx
    ret
