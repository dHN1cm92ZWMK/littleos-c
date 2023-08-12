#include "mem.h"

void* free_mem = (void*)0x7e00;

void memcpy(void *dst, const void* src, unsigned long size) {
	char *d = (char*)dst;
	const char *s = (const char*)src;
	for (;size; size--, *d++ = *s++);
}

void memset(void *dst, unsigned long size, unsigned char value) {
	char *d = (char*)dst;
	for(;size; size--, *d++ = value);
}

void* kmalloc(unsigned long size) {
	void *mem = free_mem;
	free_mem += size;
	return mem;
}
