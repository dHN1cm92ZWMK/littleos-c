#ifndef __SETUP_H__
#define __SETUP_H__

#include "mem.h"

// GDT
struct gdt_table {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct gdt_entry { // todo :
	unsigned short limit0;
	unsigned short base0;
	unsigned char base1;
	unsigned char access;
	unsigned char flags_limit;
	unsigned char base2;
}__attribute__((packed));

void set_gdt_entry(struct gdt_entry *ptr, 
		unsigned int base, 
		unsigned int limit,
		unsigned char flags,
		unsigned char access); 
void load_gdt(const struct gdt_table *gdt);

// IDT
struct idt_table {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct idt_entry {
	unsigned short offset0;
	unsigned short segment;
	unsigned char reserved;
	unsigned char flags;
	unsigned short offset1;
};

void set_idt_entry(struct idt_entry *ptr,
		unsigned int offset,
		unsigned short segment,
		unsigned char flags); 
void load_idt(const struct idt_table *idt);
#endif
