#ifndef __MEM_H__
#define __MEM_H__

void memcpy(void *dst, const void* src, unsigned long size);
void memset(void *dst, unsigned long size, unsigned char value);
void* kmalloc(unsigned long size);

#endif
