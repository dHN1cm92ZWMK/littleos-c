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
