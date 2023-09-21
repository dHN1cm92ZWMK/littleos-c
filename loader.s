global loader
global outb
global inb
global io_wait
global load_gdt
global load_idt

global dummy_interrupt_handler
global halting_interrupt_handler
global keyboard_interrupt_handler

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

io_wait:
	xor al, al
	out 0x80, al
	ret

inb:
	mov dx, [esp + 4]
	in al, dx
	ret

;dummy_handler: ; dummy, just outputs to serial and halts
;	push 0x24 ; $
;	push 0x3f8
;	call outb
;	call outb
;	call outb
;	nop
;	hlt
;	iret

dummy_interrupt_handler: ; doesn't do anything
	pusha
	popa
	iret

halting_interrupt_handler: ; just halts
	hlt

keyboard_interrupt_handler:
	pusha


	push 0x7e
	push 0x3f8
	call outb
	call outb
	call outb

	pop ax
	pop ax
	xor eax, eax

	; read from keyboard
	in al, 0x60
	;mov ebx, 0x12345
	;mov byte [ebx], al

	push eax
	xor ebx, ebx
	mov bl, al

	; DBG output to serial
	; first digit
	shr bl, 4
	cmp bl, 9
	jg .letter1
	add bl, 0x30
	jmp .print1
.letter1:
	add bl, 0x57 ; 0xe -> 'e' etc. 
	
.print1:
	push ebx ; ascii code
	push 0x3f8
	call outb
	add esp, 8 

	; second digit
	pop ebx
	and bl, 0xf
	cmp bl, 9
	jg .letter2
	add bl, 0x30
	jmp .print2
.letter2:
	add bl, 0x57
	
	; DBG output to serial
.print2:
	push ebx ; ascii code
	push 0x3f8
	call outb
	add esp, 8 

	; ack handling
	mov al, 0x20
	out 0x20, al

	popa
	add esp, 4 ; why do I need to do this?
	;hlt ; halt to check stack now
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

