#include "display.h"
#include "mem.h"
#include "string.h"
#include "serial.h"
#include "setup.h"
#include "io.h"

void gdt_loading_demo();
void idt_loading_demo();

unsigned long kmain() {
	clear_screen();

	char* s = kmalloc(10);
	memset(s, 10, 0);
	s[0] = 'h';
	s[1] = 'E';
	s[2] = 'L';
	s[3] = 'L';
	s[4] = 'o';
	putstr(0, 0, s);

	/*
	putstr(0, 22, "former line 22");
	putstr(0, 23, "former line 23");
	putstr(0, 24, "former line 24");
	memcpy((void*)(0xb8000 + 21 * 80 * 2), (void*)(0xb8000 + 22 * 80 * 2), 80 * 2);
	memcpy((void*)(0xb8000 + 22 * 80 * 2), (void*)(0xb8000 + 23 * 80 * 2), 80 * 2);
	memcpy((void*)(0xb8000 + 23 * 80 * 2), (void*)(0xb8000 + 24 * 80 * 2), 80 * 2);
	memset((void*)(0xb8000 + 24 * 80 * 2), 80 * 2, 0);
	*/
	//roll_put_str(":PPPP");
	roll_putstr("11111");
	roll_putstr("222222");
	roll_putstr("3.3.3.3.3.");

	/*
	unsigned int eax;
	somefun();
	asm volatile ("mov %%eax, %0" : "=r" (eax));
	putstr(0, 15, itoa(eax, 10));


	int a = 72;//, b = 928347;

	void* objects[] = {&a, &eax};
	putstr(0, 10, sprintf("n10={d}, n16=0x{x}", objects));
	*/

	putch(0, 24, 'x');
	putch(79, 24, 'x');
	putch(0, 0, 'x');
	putch(79, 0, 'x');

	set_cursor(0, 24);

	putch(0, 24, 'X');
	serial_test();
	putch(0, 24, 'y');

	idt_loading_demo();

	return 0x12345678;
}

void gdt_loading_demo() {
	struct gdt_entry *gdt = kmalloc(6 * sizeof(struct gdt_entry));
	set_gdt_entry(gdt + 0, 0, 0xfff0, 0, 0); // first one is zero
	set_gdt_entry(gdt + 1, 0, 0xffffff, 0xc, 0x9a); // code segment
	set_gdt_entry(gdt + 2, 0, 0xffffff, 0xc, 0x92); // data segment
	set_gdt_entry(gdt + 3, 0, 0xffffff, 0xc, 0x92); // data segment
	set_gdt_entry(gdt + 4, 0, 0xffffff, 0xc, 0x92); // data segment
	set_gdt_entry(gdt + 5, 0, 0xffffff, 0xc, 0x92); // data segment
							
	struct gdt_table *gdt_table = kmalloc(sizeof(struct gdt_table));
	gdt_table->limit = 7 * 0x8;
	gdt_table->base = (unsigned int)gdt;
	
	load_gdt(gdt_table);

	asm volatile ("mov $0x28, %eax\nmov %ax, %ds\nhlt"); // set the sixth to DS and halt
}

void dummy_handler();
void idt_loading_demo() {
	struct idt_entry *idt = kmalloc(256 * sizeof(struct idt_entry));
	for(int i = 0; i < 256; i++) {
		set_idt_entry(idt + i, (unsigned int)dummy_handler, 0x08, 0x8e); // 8e=kernel 32 bit interrupt
	}
	struct idt_table *idt_table = kmalloc(sizeof(struct idt_table));
	idt_table->limit = 0x3ff;
	idt_table->base = (unsigned int)idt;
	load_idt(idt_table);

	asm volatile ("int $80"); // invoke any interrupt; it should enter to dummy handler and halt there
}
