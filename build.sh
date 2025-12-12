echo "=== Welcome to builder ==="

rm -f src/kernel/o/*.o boot/*.bin iso/boot/* 2>/dev/null
rm -f zinc-0.0.1.iso 2>/dev/null

echo "Compiling..."

nasm -f elf32 src/kernel/kernel.asm -o src/kernel/o/kernel_asm.o
nasm -f elf32 src/kernel/keyboard/keyboard.asm -o src/kernel/o/keyboard.o

gcc -m32 -ffreestanding -nostdlib -O2 -Wall -Wextra -c src/kernel/kernel.c -o src/kernel/o/kernel.o

echo "Linking..."
ld -m elf_i386 -T src/linker.ld -o boot/mykernel.bin \
    src/kernel/o/kernel_asm.o \
    src/kernel/o/keyboard.o \
    src/kernel/o/kernel.o

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
