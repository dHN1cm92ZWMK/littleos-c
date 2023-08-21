global loader
global outb
global inb
global load_gdt
global load_idt
global dummy_handler
extern kmain

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER
KERNEL_STACK_SIZE equ 4096 ; 4 kB stack

section .bss
align 4
kernel_stack:
	resb KERNEL_STACK_SIZE


section .text
align 4
	dd MAGIC_NUMBER
	dd FLAGS
	dd CHECKSUM

;e820:
;	mov di, 0x8000
;	xor ebx, ebx
;	mov edx, 0x534d4150
;	mov ecx, 24
;	mov eax, 0xe820

load_gdt:
	mov eax, [esp+4] 
	lgdt [eax]
	ret

load_idt:
	mov eax, [esp + 4]
	lidt [eax]
	ret
	
outb:
	mov al, [esp + 8]
	mov dx, [esp + 4]
	out dx, al
	ret

inb:
	mov dx, [esp + 4]
	in al, dx
	ret

dummy_handler: ; dummy, just outputs to serial and halts
	push 0x21 ; !
	push 0x3f8
	call outb
	nop
	hlt
	iret

loader:
	mov eax, 0xCAFEBABE
	mov esp, kernel_stack + KERNEL_STACK_SIZE
	call kmain

	mov edi, 0x8000
	xor ebx, ebx
	mov edx, 0x534d4150
	mov ecx, 24
	mov eax, 0xe820

.loop:
	jmp .loop

