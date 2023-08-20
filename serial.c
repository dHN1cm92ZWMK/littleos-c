#include "serial.h"

void serial_test() {
	// baud
	outb(0x3f8 + 3, 0x80);//enable dlab
	unsigned short div = 2;
	outb(0x3f8, (div >> 8) & 0xff);
	outb(0x3f8, div & 0xff);
			      
	outb(0x3f8 + 3, 3);

	int c = 0;
	while (!(inb(0x3f8 + 5) & 0x20)) c++;

	const char *msg = "Hello on serial port\r\n";
	for(;*msg; msg++) 
		outb(0x3f8, *msg);
	outb(0x3f8, '0' + c);
}


