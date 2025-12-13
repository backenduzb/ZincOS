BITS 32

section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x00000003)

section .text
global start

extern kernel_main
extern gdt_flush   ; see next step

start:
    cli             ; 1. Disable interrupts immediately
    cld             ; 2. Clear direction flag

    call gdt_flush  ; 3. Load our GDT and set data segments

    mov esp, stack_top  ; 4. Set stack after GDT

    push ebx        ; 5. Push Multiboot info pointer
    push eax        ; 6. Push Multiboot magic number

    call kernel_main    ; 7. Jump to C kernel

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
