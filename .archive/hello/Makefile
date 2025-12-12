CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
AS = nasm
ASFLAGS = -f elf32
LD = ld
LDFLAGS = -m elf_i386 -T src/linker.ld -nostdlib

OBJS = src/kernel_asm.o src/kernel.o

all: mykernel.bin

src/kernel_asm.o: src/kernel.asm
	$(AS) $(ASFLAGS) $< -o $@

src/kernel.o: src/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

mykernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o boot/mykernel.bin
	grub-file --is-x86-multiboot boot/mykernel.bin

iso: mykernel.bin
	mkdir -p iso/boot/grub
	cp boot/mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=5' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Linux" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o mylinux.iso iso/

clean:
	rm -f $(OBJS) boot/mykernel.bin
	rm -rf iso/
	rm -f mylinux.iso

run: iso
	qemu-system-x86_64 -cdrom mylinux.iso -m 512M

test: mykernel.bin
	qemu-system-x86_64 -kernel boot/mykernel.bin -m 512M
