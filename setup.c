#include "setup.h"

void set_gdt_entry(struct gdt_entry *ptr, 
		unsigned int base, 
		unsigned int limit,
		unsigned char flags,
		unsigned char access) {
	ptr->limit0 = limit & 0xffff;
	ptr->base0 = base & 0xffff;
	ptr->base1 = (base >> 16) & 0xff;
	ptr->access = access;
	ptr->flags_limit = (flags << 4) | ((limit >> 16) & 0x0f);
	ptr->base2 = (base >> 24) & 0xff;
}

void set_idt_entry(struct idt_entry *ptr,
		unsigned int offset,
		unsigned short segment,
		unsigned char flags) {
	ptr->offset0 = offset & 0xffff;
	ptr->offset1 = offset >> 16;
	ptr->segment = segment;
	ptr->flags = flags;
}

void dummy_interrupt_handler();
void halting_interrupt_handler();
void keyboard_interrupt_handler();
void setup_idt() {
	// 256 interrupt descriptors
	struct idt_entry *idt = kmalloc(256 * sizeof(struct idt_entry));
	for(int i = 0; i < 256; i++) {
		switch (i) {
			case 0x21:
				set_idt_entry(idt + i, (unsigned int)keyboard_interrupt_handler, 0x08, 0x8e); // 8e=kernel 32 bit interrupt
				break;
			default:
				set_idt_entry(idt + i, (unsigned int)dummy_interrupt_handler, 0x08, 0x8e);
				break;

		}
	}


	// load the IDT
	struct idt_table *idt_table = kmalloc(sizeof(struct idt_table));
	idt_table->limit = 0x7ff; // 256 * 8 = 0x800
	idt_table->base = (unsigned int)idt;
	load_idt(idt_table);
}

void setup_pic() {
	// ICW1, master and slave init
	outb(0x20, 0x11);
	io_wait();
	outb(0xA0, 0x11);
	io_wait();

	// ICW2, interrupt offsets for master (0x20) and slave (0x28)
	outb(0x21, 0x20); 
	io_wait();
	outb(0xA1 , 0x28); 
	io_wait();

	// ICW3, slave on IR 2
	outb(0x21 , 0x04); 
	io_wait();
	outb(0xA1 , 0x02);
	io_wait();

	// ICW4
	outb(0x21 , 0x01);
	io_wait();
	outb(0xA1 , 0x01);
	io_wait();

	// masking
	// Only listen to keyboard irqs (IRQ 1) 
	outb( 0x21 , 0xfd);
	io_wait();
	outb( 0xA1 , 0xff);
	io_wait();
}
