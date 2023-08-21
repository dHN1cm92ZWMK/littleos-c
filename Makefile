OBJECTS = loader.o serial.o string.o display.o mem.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso

qemu_serial:
	mkfifo qemu_serial

run: os.iso qemu_serial
	qemu-system-i386 -d int,cpu_reset -D ./qemu.log -no-reboot --drive media=cdrom,file=os.iso,readonly=on -serial pipe:qemu_serial -monitor unix:qemu_monitor_socket,server,nowait
	# to tail on serial run e.g. `socat - pipe:qemu_serial`
	# to interact with monitor e.g. `socat - unix-connect:qemu_monitor_socket`

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso
