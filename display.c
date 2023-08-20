#include "display.h"
#include "mem.h"
#include "serial.h"

unsigned short *framebuffer_base = (unsigned short*)0xb8000;

void putch(unsigned char x, unsigned char y, char c) {
	framebuffer_base[x + y * 80] = 0x0200 | c;
}

void putstr(unsigned char x, unsigned char y, const char *str) {
	for (int i = 0; *str; i++, str++) {
		putch(x + i, y, *str);
	}
}

void clear_screen() {
	memset((void*)framebuffer_base, 80 * 24 * 2, 0);
}

void roll_putstr(const char *str) {
	for (int line=1; line <= 24; line++) {
		// todo improve, move whole block
		void *prev_line = (void*)(framebuffer_base + 80 * (line - 1));
		void *curr_line = (void*)(framebuffer_base + 80 * line);
		memcpy(prev_line, curr_line, 80);
	}
	putstr(0, 24, str);
}

#define FB_COMMAND_PORT 0x3d4
#define FB_DATA_PORT 0x3d5
#define FB_HIGH_BYTE 14
#define FB_LOW_BYTE 15

void set_cursor(unsigned char x, unsigned char y) {
	unsigned short pos = x + y * 80;
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE);
	outb(FB_DATA_PORT, (pos >> 8) & 0xff);
	outb(FB_COMMAND_PORT, FB_LOW_BYTE);
	outb(FB_DATA_PORT, pos & 0xff);
}
