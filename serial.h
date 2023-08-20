#ifndef __SERIAL_H__
#define __SERIAL_H__

extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short port);

void serial_test();

#endif
