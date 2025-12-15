echo "=== Welcome to builder ==="

rm -f src/kernel/o/*.o boot/*.bin iso/boot/* 2>/dev/null
rm -f zinc-0.0.1.iso 2>/dev/null

echo "Compiling..."

nasm -f elf32 src/kernel/kernel.asm -o src/kernel/o/kernel_asm.o
nasm -f elf32 src/kernel/keyboard/keyboard.asm -o src/kernel/o/keyboard.o
nasm -f elf32 src/kernel/functions/asm/shutdown.asm -o src/kernel/o/functions_shutdown.o
nasm -f elf32 src/kernel/config/gdt/gdt.asm -o src/kernel/o/gdt_asm.o
nasm -f elf32 src/kernel/config/idt/idt.asm -o src/kernel/o/idt_asm.o


gcc -m32 -ffreestanding  -nostdlib -O2 -Wall -Wextra -c src/kernel/kernel.c -o src/kernel/o/kernel.o
gcc -m32 -ffreestanding -c src/kernel/functions/c/screen/functions.c -o src/kernel/o/functions_screen.o
gcc -m32 -ffreestanding -c src/kernel/functions/c/string/functions.c -o src/kernel/o/functions_string.o
gcc -m32 -ffreestanding -c src/kernel/timing/sleep.c -o src/kernel/o/sleep.o
gcc -m32 -ffreestanding -c src/kernel/keyboard/keyboard.c -o src/kernel/o/keyboardc.o


echo "Linking..."
ld -T src/linker.ld -m elf_i386 -o boot/mykernel.bin \
    src/kernel/o/kernel.o \
    src/kernel/o/kernel_asm.o \
    src/kernel/o/functions_screen.o \
    src/kernel/o/functions_string.o \
    src/kernel/o/keyboard.o \
    src/kernel/o/functions_shutdown.o \
    src/kernel/o/keyboardc.o \
    src/kernel/o/sleep.o \
    src/kernel/o/gdt_asm.o \
    src/kernel/o/idt_asm.o


echo "Multiboot checking..."
if grub-file --is-x86-multiboot boot/mykernel.bin; then
    echo "✓ Multiboot header found!"
else
    echo "✗ Multiboot header not found"
    exit 1
fi

echo "Building ISO ..."
mkdir -p iso/boot/grub
cp boot/mykernel.bin iso/boot/

echo "Writing grub.cfg ..."
mkdir -p iso/boot/grub
cat > iso/boot/grub/grub.cfg << 'EOF'
set timeout=10
set default=0

menuentry "Zinc OS" {
    multiboot /boot/mykernel.bin
    boot
}
EOF

grub-mkrescue -o zinc-0.0.1.iso iso/ 2>/dev/null

if [ -f "zinc-0.0.1.iso" ]; then
    echo "=== SUCCESS! ==="
    echo "ISO file: $(pwd)/zinc-0.0.1.iso"
    echo "Used space: $(du -h zinc-0.0.1.iso | cut -f1)"
else
    echo "Error: ISO not created"
    exit 1
fi